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

// Qt
#include <QObject>

// Us
#include "sobjectid.h"
#include "sobjectid_p.h"

SObjectId::SObjectId()
     : d(new Private())
{
}

SObjectId::~SObjectId()
{
}

SObjectId::SObjectId(const SObjectId &rhs) : d(rhs.d)
{
}

SObjectId &SObjectId::operator=(const SObjectId &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

SObjectLocalId SObjectId::localId() const
{
    return d->mUuid;
}

void SObjectId::setLocalId(const SObjectLocalId &id)
{
    d->mUuid = id;
}

