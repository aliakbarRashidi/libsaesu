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
#include <QVariant>

// Us
#include "sobjectremoverequest.h"
#include "sobjectremoverequest_p.h"
#include "sobjectmanager.h"
#include "sobjectmanager_p.h"

SObjectRemoveRequest::Private::Private(SAbstractObjectRequest *parent)
     : SAbstractObjectRequest::Private(parent)
{
}

SObjectRemoveRequest::Private::~Private()
{
}

void SObjectRemoveRequest::Private::start(SObjectManager *manager)
{
    QSqlDatabase db = manager->d->connection();
    QSqlQuery query(db);
    QString uuidList;

    db.transaction();

    // XXX: TODO: this is bad for memory use/perf and inefficient
    for (int i = 0; i != mObjectIds.count(); ++i) {
        uuidList += QLatin1Char('\'');
        uuidList += mObjectIds.at(i).toString();
        uuidList += QLatin1Char('\'');
        
        if (i < mObjectIds.count() - 1)
            uuidList += QLatin1Char(',');

        query.prepare("INSERT INTO deletelist (key, timestamp) VALUES (:key, :timestamp)");
        query.bindValue("key", mObjectIds.at(i).toString());
        query.bindValue("timestamp", QDateTime::currentMSecsSinceEpoch());
        query.exec();
    }

    // TODO: switch to integer keys and figure out a way to make this suck less
    query.exec("DELETE FROM objects WHERE key IN (" + uuidList + ")");

    bool atLeastOne = false;
    if (query.numRowsAffected() > 0) {
        atLeastOne = true;
    }
    sDebug() << "Deleting " << mObjectIds.count() << "; " << query.numRowsAffected() << " really deleted";

    db.commit();

    // TODO: we really should not emit ids we didn't actually remove.'
    if (atLeastOne) {
        emit manager->objectsRemoved(mObjectIds);

        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);
        ds << mObjectIds;
        manager->d->mIpcChannel.sendMessage(QLatin1String("removed(QList<SObjectLocalId>)"), ba);
    }

    emit q->finished();
}
