/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
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
    sDebug() << "Executing DELETE FROM objects WHERE key IN (" + uuidList + ")";
    query.exec("DELETE FROM objects WHERE key IN (" + uuidList + ")");

    bool atLeastOne = false;
    if (query.numRowsAffected() > 0) {
        atLeastOne = true;
    }

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
