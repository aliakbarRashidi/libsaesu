/*
 * Copyright (C) 2011 Robin Burchell <robin.burchell@collabora.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
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

QUuid SObject::uuid() const
{
    return d->mUuid;
}

void SObject::setUuid(const QUuid &uuid)
{
    d->mUuid = uuid;
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
    out << item.uuid();
    out << item.d->mValues;
    return out;
}

QDataStream &operator>>(QDataStream &in, SObject &item)
{
    in >> item.d->mUuid;
    in >> item.d->mValues;

    return in;
}

