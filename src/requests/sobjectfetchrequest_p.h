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

#ifndef SOBJECTFETCHREQUEST_P_H
#define SOBJECTFETCHREQUEST_P_H

// Qt
#include <QObject>

// Us
#include "sglobal.h"
#include "sobject.h"
#include "sobjectfetchrequest.h"
#include "sabstractobjectrequest_p.h"

class SObjectFetchRequest::Private : public SAbstractObjectRequest::Private
{
    Q_OBJECT
public:
    explicit Private(SAbstractObjectRequest *parent = 0);

    void start(SObjectManager *manager);

    QList<SObject> mObjects;
    SAbstractObjectFilter mFilter;
};

#endif // SOBJECTFETCHREQUEST_P_H
