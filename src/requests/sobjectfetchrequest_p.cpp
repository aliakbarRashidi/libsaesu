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
    query.exec("SELECT object, timestamp, hash, key FROM objects");
    
    while (query.next()) {
        QByteArray b(query.value(0).toByteArray());
        QDataStream ds(&b, QIODevice::ReadOnly);
        SObject obj;
        ds >> obj.d->mValues;
        obj.d->mLastSaved = query.value(1).toLongLong();
        obj.d->mHash = query.value(2).toByteArray();
        obj.d->mId.setLocalId(QUuid(query.value(3).toString()));

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

