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

#ifndef SOBJECTDETAILFILTER_P_H
#define SOBJECTDETAILFILTER_P_H

// Qt
#include <QObject>
#include <QString>

// Us
#include "sobjectdetailfilter.h"
#include "sabstractobjectfilter.h"
#include "sabstractobjectfilter_p.h"

class SObjectDetailFilter::Private : public SAbstractObjectFilter::Private
{
public:
    explicit Private();
    virtual ~Private();

    bool matches(SObject *object);

    QString mDetailName;
    QVariant mDetailValue;
};

#endif // SOBJECTDETAILFILTER_H
