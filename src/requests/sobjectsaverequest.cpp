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

// Us
#include "sobjectsaverequest.h"
#include "sobjectsaverequest_p.h"
#include "sobject.h"

SObjectSaveRequest::SObjectSaveRequest(QObject *parent)
    : SAbstractObjectRequest(parent)
{
    d = new Private(this);
}

SObjectSaveRequest::~SObjectSaveRequest()
{
}

void SObjectSaveRequest::setObjects(const QList<SObject> &objects)
{
    static_cast<Private*>(d)->mObjects = objects;
}

void SObjectSaveRequest::setObject(const SObject &object)
{
    static_cast<Private*>(d)->mObjects = QList<SObject>() << object;
}

QList<SObject> SObjectSaveRequest::objects() const
{
    return static_cast<Private*>(d)->mObjects;
}

void SObjectSaveRequest::add(const SObject &object)
{
    static_cast<Private*>(d)->mObjects.append(object);
}

void SObjectSaveRequest::setSaveHint(SaveHint saveHint)
{
    static_cast<Private*>(d)->mSaveHint = saveHint;
}

SObjectSaveRequest::SaveHint SObjectSaveRequest::saveHint() const
{
    return static_cast<Private*>(d)->mSaveHint;
}
