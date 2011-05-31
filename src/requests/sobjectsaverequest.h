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

#ifndef SOBJECTSAVEREQUEST_H
#define SOBJECTSAVEREQUEST_H

// Qt
#include <QObject>

// Us
#include "sglobal.h"
#include "sobject.h"
#include "sabstractobjectrequest.h"

class SObjectSaveRequest : public SAbstractObjectRequest
{
    Q_OBJECT
public:
    explicit SObjectSaveRequest(QObject *parent = 0);
    ~SObjectSaveRequest();

    void setObjects(const QList<SObject> &objects);
    void setObject(const SObject &object);
    void add(const SObject &object);
    QList<SObject> objects() const;

    enum SaveHint
    {
        SaveHintNone = 0,
        ObjectFromSync
    };

    void setSaveHint(SaveHint saveHint);
    SaveHint saveHint() const;

private:
    class Private;
};

#endif // SOBJECTSAVEEQUEST_H
