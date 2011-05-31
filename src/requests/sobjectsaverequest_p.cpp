/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, and all associated material(s), including but not limited
 * to documentation are protected by copyright. All rights reserved.
 * Copying, including reproducing, storing, adapting, translating, or
 * reverse-engineering any or all of this material requires prior written
 * consent. This material also contains confidential information which
 * may not be disclosed in any form without prior written consent.
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

#undef SOBJECTSAVEREQUEST_DEBUG

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

        // TODO: this logic is wrong - if we recieve an object from a remote peer,
        // it may have an id set, but be an added
        // row, at least, according to our point of view.
        //
        // we probably need to do checking on the insert/replace query using
        // sqlite3_changes() to see if a row was actually inserted or not
        if (obj.id().localId().isNull()) {
            obj.id().setLocalId(QUuid::createUuid());
            objectsAdded.append(obj.id().localId());
        } else {
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
#ifdef SOBJECTSAVEREQUEST_DEBUG
            sDebug() << "Keeping existing save timestamp of " << obj.lastSaved();
#endif
        } else {
            query.bindValue(":timestamp", QDateTime::currentMSecsSinceEpoch());
#ifdef SOBJECTSAVEREQUEST_DEBUG
            sDebug() << "Setting save timestamp to " << QDateTime::currentMSecsSinceEpoch();
#endif
        }

        query.bindValue(":key", obj.id().localId().toString());
        query.bindValue(":hash", hasher.result());
        
        QByteArray buf;
        QDataStream ds(&buf, QIODevice::ReadWrite);
        ds << obj.d->mValues;
        
        query.bindValue(":data", buf);
        query.exec();

#ifdef SOBJECTSAVEREQUEST_DEBUG
        sDebug() << "Serialised form " << buf.toHex();
#endif
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

#ifdef SOBJECTSAVEREQUEST_DEBUG
    sDebug() << "Done";
#endif
    emit q->finished();
}
