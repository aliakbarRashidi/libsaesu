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

#ifndef SOBJECTLOCALIDFILTER_H
#define SOBJECTLOCALIDFILTER_H

// Qt
#include <QObject>
#include <QString>
#include <QUuid>

#include "sobjectid.h"
#include "sabstractobjectfilter.h"

class SObjectLocalIdFilter : public SAbstractObjectFilter
{
public:
    explicit SObjectLocalIdFilter();
    virtual ~SObjectLocalIdFilter();

    void setIds(const QList<SObjectLocalId> &ids);
    QList<SObjectLocalId> ids() const;
    void add(const SObjectLocalId &id);
    void remove(const SObjectLocalId &id);
    void clear();

private:
    class Private;
};

#endif // SOBJECTLOCALIDFILTER_H
