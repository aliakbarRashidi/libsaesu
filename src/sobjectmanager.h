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

#ifndef SOBJECTMANAGER_H
#define SOBJECTMANAGER_H

// Qt
#include <QObject>
#include <QString>

// Us
class SAbstractObjectRequest;
#include "sobjectid.h"

class SObjectManager : public QObject
{
    Q_OBJECT
public:
    explicit SObjectManager(const QString &tableName, QObject *parent = 0);
    virtual ~SObjectManager();

signals:
    void objectsAdded(const QList<SObjectLocalId> &objectIds);
    void objectsUpdated(const QList<SObjectLocalId> &objectIds);
    void objectsRemoved(const QList<SObjectLocalId> &objectIds);

private:
    friend class SObjectSaveRequest;
    friend class SObjectRemoveRequest;
    friend class SObjectFetchRequest;
    friend class SDeleteListFetchRequest;

    class Private;
    Private *d;
};

#endif // SOBJECTMANAGER_H
