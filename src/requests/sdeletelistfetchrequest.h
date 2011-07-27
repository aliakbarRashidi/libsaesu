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

#ifndef SDELETELISTFETCHREQUEST_H
#define SDELETELISTFETCHREQUEST_H

// Qt
#include <QObject>

// Us
#include "sglobal.h"
#include "sobject.h"
#include "sabstractobjectrequest.h"
#include "sabstractobjectfilter.h"

class SDeleteListFetchRequest : public SAbstractObjectRequest
{
    Q_OBJECT
public:
    explicit SDeleteListFetchRequest(QObject *parent = 0);
    virtual ~SDeleteListFetchRequest();

    QList<SObjectLocalId> objectIds() const;

private:
    class Private;
};

#endif // SDELETELISTFETCHREQUEST_H
