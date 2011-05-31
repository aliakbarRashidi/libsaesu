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

#ifndef SOBJECTREMOVEREQUEST_H
#define SOBJECTREMOVEREQUEST_H

// Qt
#include <QObject>
#include <QString>
#include <QUuid>

#include "sabstractobjectrequest.h"
#include "sobjectid.h"

class SObjectRemoveRequest : public SAbstractObjectRequest
{
public:
    explicit SObjectRemoveRequest(QObject *parent = 0);
    virtual ~SObjectRemoveRequest();

    void add(const SObjectLocalId &id);
    void setObjectId(const SObjectLocalId &id);
    void setObjectIds(const QList<SObjectLocalId> &ids);
    QList<QUuid> objectIds();

private:
    class Private;
};

#endif // SOBJECTREMOVEREQUEST_H
