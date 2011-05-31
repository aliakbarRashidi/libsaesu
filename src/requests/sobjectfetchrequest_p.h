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

#ifndef SOBJECTFETCHREQUEST_P_H
#define SOBJECTFETCHREQUEST_P_H

// Qt
#include <QObject>

// Us
#include "sglobal.h"
#include "sobject.h"
#include "sobjectfetchrequest.h"
#include "sabstractobjectrequest_p.h"

class SObjectFetchRequest::Private : public SAbstractObjectRequest::Private
{
    Q_OBJECT
public:
    explicit Private(SAbstractObjectRequest *parent = 0);

    void start(SObjectManager *manager);

    QList<SObject> mObjects;
    SAbstractObjectFilter mFilter;
};

#endif // SOBJECTFETCHREQUEST_P_H
