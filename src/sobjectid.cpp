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
#include <QObject>
#include <QMetaType>

// Us
#include "sobjectid.h"

SObjectId::SObjectId()
{
    // XXX: this is a bit of an ugly hack
    static bool initialisedTypes = false;
    if (!initialisedTypes) {
        qRegisterMetaType<SObjectLocalId>("SObjectLocalId");
        qRegisterMetaType<QList<SObjectLocalId> >("QList<SObjectLocalId>");
        qRegisterMetaType<SObjectId>("SObjectId");
        qRegisterMetaType<QList<SObjectId> >("QList<SObjectId>");
    }
}

SObjectId::~SObjectId()
{
}

SObjectLocalId SObjectId::localId() const
{
    return mUuid;
}

void SObjectId::setLocalId(const SObjectLocalId &id)
{
    mUuid = id;
}

