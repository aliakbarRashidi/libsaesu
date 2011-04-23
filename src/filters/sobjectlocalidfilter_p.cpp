/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
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
