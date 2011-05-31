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

#ifndef SOBJECTREMOVEREQUEST_P_H
#define SOBJECTREMOVEREQUEST_P_H

// Qt
#include <QObject>
#include <QString>

// Us
#include "sabstractobjectrequest_p.h"
#include "sobjectremoverequest.h"

class SObjectRemoveRequest::Private : public SAbstractObjectRequest::Private
{
public:
    explicit Private(SAbstractObjectRequest *parent);
    virtual ~Private();

    void start(SObjectManager *manager);

    QList<QUuid> mObjectIds;
};

#endif // SOBJECTREMOVEREQUEST_H
