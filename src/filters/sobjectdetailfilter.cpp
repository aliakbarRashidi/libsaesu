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
#include "sobjectdetailfilter.h"
#include "sobjectdetailfilter_p.h"

SObjectDetailFilter::SObjectDetailFilter()
     : SAbstractObjectFilter()
{
    d = QSharedPointer<SAbstractObjectFilter::Private>(new SObjectDetailFilter::Private);
}

SObjectDetailFilter::~SObjectDetailFilter()
{
}

void SObjectDetailFilter::setDetailName(const QString &name)
{
    static_cast<Private*>(d.data())->mDetailName = name;
}

QString SObjectDetailFilter::detailName() const
{
    return static_cast<Private*>(const_cast<SAbstractObjectFilter::Private*>(d.data()))->mDetailName;
}

void SObjectDetailFilter::setValue(const QVariant &value)
{
    static_cast<Private*>(d.data())->mDetailValue = value;
}

QVariant SObjectDetailFilter::value() const
{
    return static_cast<Private*>(const_cast<SAbstractObjectFilter::Private*>(d.data()))->mDetailValue;
}
