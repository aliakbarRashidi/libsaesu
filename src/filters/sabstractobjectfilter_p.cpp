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

#include "sabstractobjectfilter.h"
#include "sabstractobjectfilter_p.h"

SAbstractObjectFilter::Private::Private()
{
}

SAbstractObjectFilter::Private::~Private()
{
}

bool SAbstractObjectFilter::Private::matches(SObject *object)
{
    // default filter: check nothing
    sDebug() << "Default filter; matching everything";
    return true;
}
