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

#ifndef SOBJECTID_H
#define SOBJECTID_H

// Qt
#include <QSharedPointer>
#include <QUuid>

typedef QUuid SObjectLocalId;

class SObjectId
{
public:
    SObjectId();
    SObjectId(const SObjectId &);
    SObjectId &operator=(const SObjectId &);
    virtual ~SObjectId();

    void setLocalId(const SObjectLocalId &id);
    SObjectLocalId localId() const;

private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // SOBJECTID_H
