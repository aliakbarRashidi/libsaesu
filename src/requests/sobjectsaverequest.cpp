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

// Us
#include "sobjectsaverequest.h"
#include "sobjectsaverequest_p.h"
#include "sobject.h"

SObjectSaveRequest::SObjectSaveRequest(QObject *parent)
    : SAbstractObjectRequest(parent)
{
    d = new Private(this);
}

SObjectSaveRequest::~SObjectSaveRequest()
{
}

void SObjectSaveRequest::setObjects(const QList<SObject> &objects)
{
    static_cast<Private*>(d)->mObjects = objects;
}

void SObjectSaveRequest::setObject(const SObject &object)
{
    static_cast<Private*>(d)->mObjects = QList<SObject>() << object;
}

QList<SObject> SObjectSaveRequest::objects() const
{
    return static_cast<Private*>(d)->mObjects;
}

void SObjectSaveRequest::add(const SObject &object)
{
    static_cast<Private*>(d)->mObjects.append(object);
}

void SObjectSaveRequest::setSaveHint(SaveHint saveHint)
{
    static_cast<Private*>(d)->mSaveHint = saveHint;
}

SObjectSaveRequest::SaveHint SObjectSaveRequest::saveHint() const
{
    return static_cast<Private*>(d)->mSaveHint;
}
