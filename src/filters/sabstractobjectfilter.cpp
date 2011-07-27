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

