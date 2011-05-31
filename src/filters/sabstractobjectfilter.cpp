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

// Us
#include "sabstractobjectfilter.h"
#include "sabstractobjectfilter_p.h"

SAbstractObjectFilter::SAbstractObjectFilter()
{
}

SAbstractObjectFilter::SAbstractObjectFilter(const SAbstractObjectFilter &rhs) : d(rhs.d)
{
}

SAbstractObjectFilter &SAbstractObjectFilter::operator=(const SAbstractObjectFilter &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

SAbstractObjectFilter::~SAbstractObjectFilter()
{
}

bool SAbstractObjectFilter::matches(SObject *object)
{
    if (!d)
        return true; // no filter: match everything

    return d->matches(object);
}

