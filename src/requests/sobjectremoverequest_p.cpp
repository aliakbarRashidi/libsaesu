/*
 * Copyright (C) 2010-2011 Robin Burchell
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

#if 0
    // TODO: we really should not emit ids we didn't actually remove.'
    if (atLeastOne) {
        emit manager->objectsRemoved(mObjectIds);

        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);
        ds << mObjectIds;
        manager->d->mIpcChannel.sendMessage(QLatin1String("removed(QList<SObjectLocalId>)"), ba);
    }
#endif

    emit q->finished();
}
