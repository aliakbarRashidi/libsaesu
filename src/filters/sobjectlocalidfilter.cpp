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

