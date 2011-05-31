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

#ifndef SOBJECTDETAILFILTER_H
#define SOBJECTDETAILFILTER_H

// Qt
#include <QObject>
#include <QString>
#include <QVariant>

#include "sabstractobjectfilter.h"

class SObjectDetailFilter : public SAbstractObjectFilter
{
public:
    explicit SObjectDetailFilter();
    virtual ~SObjectDetailFilter();

    void setDetailName(const QString &name);
    QString detailName() const;

    void setValue(const QVariant &value);
    QVariant value() const;

    // TODO: support match types other than an exact match

private:
    class Private;
};

#endif // SOBJECTDETAILFILTER_H
