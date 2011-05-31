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

#ifndef SABSTRACTOBJECTREQUEST_H
#define SABSTRACTOBJECTREQUEST_H

#include <QObject>

#include "sglobal.h"
class SObjectManager;

class SAbstractObjectRequest : public QObject
{
    Q_OBJECT
public:
    explicit SAbstractObjectRequest(QObject *parent = 0);

    /*! Performs the request.
     */
    void start(SObjectManager *manager);

signals:
    void started();
    void finished();

protected:
    friend class SObjectManager;
    friend class SObjectSaveRequest;
    friend class SObjectFetchRequest;
    friend class SObjectRemoveRequest;
    friend class SDeleteListFetchRequest;

    class Private;
    Private *d;
};

#endif // SABSTRACTOBJECTREQUEST_H
