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

#include "sobjectfetchrequest.h"
#include "sobjectfetchrequest_p.h"
#include "sobjectmanager.h"
#include "sobjectmanager_p.h"
#include "sobject.h"
#include "sobject_p.h"

SObjectFetchRequest::Private::Private(SAbstractObjectRequest *parent)
    : SAbstractObjectRequest::Private(parent)
{
}

void SObjectFetchRequest::Private::start(SObjectManager *manager)
{
    QSqlDatabase db = manager->d->connection();
    QSqlQuery query(db);
    query.exec("SELECT object, timestamp, hash FROM objects");
    
    while (query.next()) {
        QByteArray b(query.value(0).toByteArray());
        QDataStream ds(&b, QIODevice::ReadOnly);
        SObject obj;
        ds >> obj;
        obj.d->mLastSaved = query.value(1).toLongLong();
        obj.d->mHash = query.value(2).toByteArray();

        // TODO: optimise for the case of no filter
        // TODO: we should probably pass a list of objects and have the filter
        // remove the ones it doesn't match instead of checking each
        // object seperately
        //
        // TODO: optimise id filters to select the rows matching instead of all objs
        if (mFilter.matches(&obj))
            mObjects << obj;

//        sDebug() << "Read " << obj.uuid();
    }

    emit q->finished();
}

