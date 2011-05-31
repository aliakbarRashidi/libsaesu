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

// Qt
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>

#include "sdeletelistfetchrequest.h"
#include "sdeletelistfetchrequest_p.h"
#include "sobjectmanager.h"
#include "sobjectmanager_p.h"
#include "sobject.h"
#include "sobject_p.h"

SDeleteListFetchRequest::Private::Private(SAbstractObjectRequest *parent)
    : SAbstractObjectRequest::Private(parent)
{
}

void SDeleteListFetchRequest::Private::start(SObjectManager *manager)
{
    QSqlDatabase db = manager->d->connection();
    QSqlQuery query(db);
    query.exec("SELECT key FROM deletelist");
    
    while (query.next()) {
        SObjectLocalId id(query.value(0).toString());
        mObjectIds << id;
    }

    emit q->finished();
}

