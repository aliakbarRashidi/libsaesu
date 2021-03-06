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

#ifndef SOBJECTREMOVEREQUEST_P_H
#define SOBJECTREMOVEREQUEST_P_H

// Qt
#include <QObject>
#include <QString>

// Us
#include "sabstractobjectrequest_p.h"
#include "sobjectremoverequest.h"

class SObjectRemoveRequest::Private : public SAbstractObjectRequest::Private
{
public:
    explicit Private(SAbstractObjectRequest *parent);
    virtual ~Private();

    void start(SObjectManager *manager);

    QList<QUuid> mObjectIds;
};

#endif // SOBJECTREMOVEREQUEST_H
