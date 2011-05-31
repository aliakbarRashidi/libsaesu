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
#include <QSharedData>

// Us
#include "sobject.h"
#include "sobject_p.h"

SObject::SObject() : d(new Private)
{
}

SObject::SObject(const SObject &rhs) : d(rhs.d)
{
}

SObject &SObject::operator=(const SObject &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

SObject::~SObject()
{
}

QByteArray SObject::hash() const
{
    return d->mHash;
}

qint64 SObject::lastSaved() const
{
    return d->mLastSaved;
}

SObjectId SObject::id() const
{
    return d->mId;
}

void SObject::setId(const SObjectId &id)
{
    d->mId = id;
}

QVariant SObject::value(const QString &fieldName) const
{
    return d->mValues[fieldName];
}

void SObject::setValue(const QString &fieldName, const QVariant &value)
{
    d->mValues[fieldName] = value;
}

QDataStream &operator<<(QDataStream &out, const SObject &item)
{
    // TODO: versioning
    out << item.id().localId();
    out << item.d->mLastSaved;
    out << item.d->mValues;
    return out;
}

QDataStream &operator>>(QDataStream &in, SObject &item)
{
    QUuid id;
    in >> id;
    item.d->mId.setLocalId(id);
    in >> item.d->mLastSaved;
    in >> item.d->mValues;

    return in;
}

