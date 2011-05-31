/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, and all associated material(s), including but not limited
 * to documentation are protected by copyright. All rights reserved.
 * Copying, including reproducing, storing, adapting, translating, or
 * reverse-engineering any or all of this material requires prior written
 * consent. This material also contains confidential information which
 * may not be disclosed in any form without prior written consent.
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
