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

#ifndef SABSTRACTOBJECTREQUEST_P_H
#define SABSTRACTOBJECTREQUEST_P_H

#include <QObject>

#include "sabstractobjectrequest.h"

class SAbstractObjectRequest::Private : public QObject
{
    Q_OBJECT
public:
    explicit Private(SAbstractObjectRequest *parent = 0);

    /*! Performs the request.
     */
    virtual void start(SObjectManager *manager) = 0;

    SAbstractObjectRequest *q;
};

#endif // SABSTRACTOBJECTREQUEST_P_H
