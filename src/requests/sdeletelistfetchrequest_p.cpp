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

#include "sdeletelistfetchrequest.h"
#include "sdeletelistfetchrequest_p.h"
#include "sobjectmanager.h"
#include "sobjectmanager_p.h"
#include "sobject.h"
#include "sobject_p.h"

SDeleteListFetchRequest::Private::Private(SAbstractObjectRequest *parent)
    : SAbstractObjectRequest::Private(parent)
{
}

void SDeleteListFetchRequest::Private::start(SObjectManager *manager)
{
    QSqlDatabase db = manager->d->connection();
    QSqlQuery query(db);
    query.exec("SELECT key FROM deletelist");
    
    while (query.next()) {
        SObjectLocalId id(query.value(0).toString());
        mObjectIds << id;
    }

    emit q->finished();
}

