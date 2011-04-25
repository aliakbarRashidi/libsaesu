/*
 * Copyright (C) 2011 Robin Burchell <robin.burchell@collabora.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

// Qt
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QCryptographicHash>

// Us
#include "sobjectsaverequest.h"
#include "sobjectsaverequest_p.h"
#include "sobject.h"
#include "sobject_p.h"
#include "sobjectmanager.h"
#include "sobjectmanager_p.h"

SObjectSaveRequest::Private::Private(SAbstractObjectRequest *parent)
    : SAbstractObjectRequest::Private(parent)
    , mSaveHint(SaveHintNone)
{
}

void SObjectSaveRequest::Private::start(SObjectManager *manager)
{
    QSqlDatabase db = manager->d->connection();
    QSqlQuery query(db);
    QList<SObjectLocalId> objectsAdded;
    QList<SObjectLocalId> objectsUpdated;

    for (int i = 0; i < mObjects.count(); ++i) {
        SObject &obj = mObjects[i];

        if (obj.id().localId().isNull()) {
            obj.id().setLocalId(QUuid::createUuid());

            // insert
            sDebug() << "Inserting " << obj.id().localId();
            objectsAdded.append(obj.id().localId());
        } else {
            sDebug() << "Updating " << obj.id().localId();
            objectsUpdated.append(obj.id().localId());
        }

        // calculate data hash
        QCryptographicHash hasher(QCryptographicHash::Sha1);

        QStringList keys = obj.d->mValues.keys();
        foreach (const QString &key, keys) {
            hasher.addData(key.toUtf8());
            hasher.addData(obj.d->mValues[key].toByteArray());
        }

        query.prepare("INSERT OR REPLACE INTO objects (key, timestamp, hash, object) VALUES (:key, :timestamp, :hash, :data)");

        if (mSaveHint == SObjectSaveRequest::ObjectFromSync) {
            // if the object is coming from sync, it presumably already has
            // a save timestamp, so we should reuse it
            query.bindValue(":timestamp", obj.lastSaved());
        } else {
            query.bindValue(":timestamp", QDateTime::currentMSecsSinceEpoch());
        }

        query.bindValue(":key", obj.id().localId().toString());
        query.bindValue(":hash", hasher.result());
        
        QByteArray buf;
        QDataStream ds(&buf, QIODevice::ReadWrite);
        ds << obj.d->mValues;
        
        query.bindValue(":data", buf);
        query.exec();

        sDebug() << "Serialised form " << buf.toHex();
        sDebug() << "No hex: " << buf;
    }
    
    if (!objectsAdded.isEmpty()) {
        emit manager->objectsAdded(objectsAdded);

        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);
        ds << objectsAdded;
        manager->d->mIpcChannel.sendMessage(QLatin1String("added(QList<SObjectLocalId>)"), ba);
    }

    if (!objectsUpdated.isEmpty()) {
        emit manager->objectsUpdated(objectsUpdated);

        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);
        ds << objectsUpdated;
        manager->d->mIpcChannel.sendMessage(QLatin1String("updated(QList<SObjectLocalId>)"), ba);
    }

    db.commit();

    sDebug() << "Done";
    emit q->finished();
}
