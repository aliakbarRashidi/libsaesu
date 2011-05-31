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
#include <QObject>
#include <QMetaType>

// Us
#include "sobjectid.h"
#include "sobjectid_p.h"

SObjectId::SObjectId()
     : d(new Private())
{
    // XXX: this is a bit of an ugly hack
    static bool initialisedTypes = false;
    if (!initialisedTypes) {
        qRegisterMetaType<SObjectLocalId>("SObjectLocalId");
        qRegisterMetaType<QList<SObjectLocalId> >("QList<SObjectLocalId>");
        qRegisterMetaType<SObjectId>("SObjectId");
        qRegisterMetaType<QList<SObjectId> >("QList<SObjectId>");
    }
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

