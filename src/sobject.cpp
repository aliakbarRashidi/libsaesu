/*
 * Copyright (C) 2010-2011 Robin Burchell
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

QList<QString> SObject::fields() const
{
    return d->mValues.keys();
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

