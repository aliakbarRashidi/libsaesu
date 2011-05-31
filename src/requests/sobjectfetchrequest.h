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

#ifndef SOBJECTFETCHREQUEST_H
#define SOBJECTFETCHREQUEST_H

// Qt
#include <QObject>

// Us
#include "sglobal.h"
#include "sobject.h"
#include "sabstractobjectrequest.h"
#include "sabstractobjectfilter.h"

class SObjectFetchRequest : public SAbstractObjectRequest
{
    Q_OBJECT
public:
    explicit SObjectFetchRequest(QObject *parent = 0);
    virtual ~SObjectFetchRequest();

    QList<SObject> objects() const;

    void setFilter(const SAbstractObjectFilter &filter);
    SAbstractObjectFilter filter() const;

private:
    class Private;
};

#endif // SOBJECTFETCHREQUEST_H
