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

#include "sobjectfetchrequest.h"
#include "sobjectfetchrequest_p.h"
#include "sobjectmanager.h"
#include "sobjectmanager_p.h"
#include "sobject.h"
#include "sobject_p.h"

SObjectFetchRequest::Private::Private(SAbstractObjectRequest *parent)
    : SAbstractObjectRequest::Private(parent)
{
}

void SObjectFetchRequest::Private::start(SObjectManager *manager)
{
    QSqlDatabase db = manager->d->connection();
    QSqlQuery query(db);
    query.exec("SELECT object, timestamp, hash, key FROM objects");
    
    while (query.next()) {
        QByteArray b(query.value(0).toByteArray());
        QDataStream ds(&b, QIODevice::ReadOnly);
        SObject obj;
        ds >> obj.d->mValues;
        obj.d->mLastSaved = query.value(1).toLongLong();
        obj.d->mHash = query.value(2).toByteArray();
        obj.d->mId.setLocalId(QUuid(query.value(3).toString()));

        // TODO: optimise for the case of no filter
        // TODO: we should probably pass a list of objects and have the filter
        // remove the ones it doesn't match instead of checking each
        // object seperately
        //
        // TODO: optimise id filters to select the rows matching instead of all objs
        if (mFilter.matches(&obj))
            mObjects << obj;

//        sDebug() << "Read " << obj.uuid();
    }

    emit q->finished();
}

