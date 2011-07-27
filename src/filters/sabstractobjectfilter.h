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

#ifndef SABSTRACTOBJECTFILTER_H
#define SABSTRACTOBJECTFILTER_H

// Qt
#include <QObject>
#include <QString>
#include <QSharedPointer>

// Us
#include "sglobal.h"
class SObject;

class SAbstractObjectFilter
{
public:
    SAbstractObjectFilter();
    SAbstractObjectFilter(const SAbstractObjectFilter &);
    SAbstractObjectFilter &operator=(const SAbstractObjectFilter &);
    virtual ~SAbstractObjectFilter();

    virtual bool matches(SObject *object);

private:
    friend class SObjectLocalIdFilter;
    friend class SObjectDetailFilter;

    class Private;
    QSharedPointer<Private> d;
};

#endif // SABSTRACTOBJECTFILTER_H
