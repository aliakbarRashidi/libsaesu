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

// Us
#include "sobjectremoverequest.h"
#include "sobjectremoverequest_p.h"

SObjectRemoveRequest::SObjectRemoveRequest(QObject *parent)
     : SAbstractObjectRequest(parent)
{
     d = new Private(this);
}

SObjectRemoveRequest::~SObjectRemoveRequest()
{
}

void SObjectRemoveRequest::setObjectId(const QUuid &id)
{
    static_cast<Private*>(d)->mObjectIds.clear();
    static_cast<Private*>(d)->mObjectIds.append(id);
}

void SObjectRemoveRequest::setObjectIds(const QList<QUuid> &ids)
{
    static_cast<Private*>(d)->mObjectIds = ids;
}

QList<QUuid> SObjectRemoveRequest::objectIds()
{
    return static_cast<Private*>(d)->mObjectIds;
}

void SObjectRemoveRequest::add(const SObjectLocalId &id)
{
    static_cast<Private*>(d)->mObjectIds.append(id);
}
