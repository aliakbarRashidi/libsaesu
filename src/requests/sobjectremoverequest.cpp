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
