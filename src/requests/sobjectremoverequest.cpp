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

// Us
#include "sobjectremoverequest.h"
#include "sobjectremoverequest_p.h"

SObjectRemoveRequest::SObjectRemoveRequest(QObject *parent)
     : SAbstractObjectRequest(parent)
{
     d = new Private(this);
}

SObjectRemoveRequest::~SObjectRemoveRequest()
{
}

void SObjectRemoveRequest::setObjectId(const QUuid &id)
{
    static_cast<Private*>(d)->mObjectIds.clear();
    static_cast<Private*>(d)->mObjectIds.append(id);
}

void SObjectRemoveRequest::setObjectIds(const QList<QUuid> &ids)
{
    static_cast<Private*>(d)->mObjectIds = ids;
}

QList<QUuid> SObjectRemoveRequest::objectIds()
{
    return static_cast<Private*>(d)->mObjectIds;
}

void SObjectRemoveRequest::add(const SObjectLocalId &id)
{
    static_cast<Private*>(d)->mObjectIds.append(id);
}
