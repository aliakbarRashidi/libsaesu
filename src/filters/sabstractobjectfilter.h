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

#ifndef SABSTRACTOBJECTFILTER_H
#define SABSTRACTOBJECTFILTER_H

// Qt
#include <QObject>
#include <QString>
#include <QSharedPointer>

// Us
#include "sglobal.h"
class SObject;

class SAbstractObjectFilter
{
public:
    SAbstractObjectFilter();
    SAbstractObjectFilter(const SAbstractObjectFilter &);
    SAbstractObjectFilter &operator=(const SAbstractObjectFilter &);
    virtual ~SAbstractObjectFilter();

    virtual bool matches(SObject *object);

private:
    friend class SObjectLocalIdFilter;
    friend class SObjectDetailFilter;

    class Private;
    QSharedPointer<Private> d;
};

#endif // SABSTRACTOBJECTFILTER_H
