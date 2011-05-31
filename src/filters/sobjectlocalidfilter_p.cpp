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

#include "sobject.h"
#include "sobjectlocalidfilter.h"
#include "sobjectlocalidfilter_p.h"

SObjectLocalIdFilter::Private::Private()
     : SAbstractObjectFilter::Private()
{
}

SObjectLocalIdFilter::Private::~Private()
{
}

bool SObjectLocalIdFilter::Private::matches(SObject *object)
{
    if (mFilterIds.contains(object->id().localId()))
        return true;
    return false;
}
