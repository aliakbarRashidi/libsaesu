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

// Us
#include "sobjectsaverequest.h"
#include "sobjectsaverequest_p.h"
#include "sobject.h"
#include "sobjectmanager.h"
#include "sobjectmanager_p.h"

SObjectSaveRequest::Private::Private(SAbstractObjectRequest *parent)
    : SAbstractObjectRequest::Private(parent)
{
}

void SObjectSaveRequest::Private::start(SObjectManager *manager)
{
    QSqlDatabase db = manager->d->connection();
    QSqlQuery query(db);
    QList<SObjectLocalId> objectsAdded;
    QList<SObjectLocalId> objectsUpdated;

    foreach (SObject obj, mObjects) {
        if (obj.id().localId().isNull()) {
            obj.id().setLocalId(QUuid::createUuid());

            // insert
            sDebug() << "Inserting " << obj.id().localId();
            objectsAdded.append(obj.id().localId());
        } else {
            sDebug() << "Updating " << obj.id().localId();
            objectsUpdated.append(obj.id().localId());
        }

        query.prepare("INSERT OR REPLACE INTO objects (key, object) VALUES (:key, :data)");
        query.bindValue(":key", obj.id().localId().toString());
        
        QByteArray buf;
        QDataStream ds(&buf, QIODevice::ReadWrite);
        ds << obj;
        
        query.bindValue(":data", buf);
        query.exec();

        sDebug() << "Serialised form " << buf.toHex();
        sDebug() << "No hex: " << buf;
    }

    if (!objectsAdded.isEmpty())
        emit manager->objectsAdded(objectsAdded);

    if (!objectsUpdated.isEmpty())
        emit manager->objectsUpdated(objectsUpdated);

    db.commit();

    sDebug() << "Done";
    emit q->finished();
}
