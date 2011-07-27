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
#include <QSharedData>

// Us
#include "sobjectlocalidfilter.h"
#include "sobjectlocalidfilter_p.h"

SObjectLocalIdFilter::SObjectLocalIdFilter()
     : SAbstractObjectFilter()
{
    d = QSharedPointer<SAbstractObjectFilter::Private>(new SObjectLocalIdFilter::Private());
}

SObjectLocalIdFilter::~SObjectLocalIdFilter()
{
}

void SObjectLocalIdFilter::setIds(const QList<SObjectLocalId> &ids)
{
    static_cast<Private*>(d.data())->mFilterIds = ids;
}

QList<SObjectLocalId> SObjectLocalIdFilter::ids() const
{
    return static_cast<Private*>(const_cast<SAbstractObjectFilter::Private*>(d.data()))->mFilterIds;
}

void SObjectLocalIdFilter::add(const SObjectLocalId &id)
{
    static_cast<Private*>(d.data())->mFilterIds.append(id);
}

void SObjectLocalIdFilter::remove(const SObjectLocalId &id)
{
    static_cast<Private*>(d.data())->mFilterIds.removeAll(id);
}

void SObjectLocalIdFilter::clear()
{
    static_cast<Private*>(d.data())->mFilterIds.clear();
}

