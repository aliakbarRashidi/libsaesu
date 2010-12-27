/*
 * Copyright (C) 2010 Robin Burchell <robin.burchell@collabora.co.uk>
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
#include <QtCore/QUuid>

// Us
#include "scloudstorage.h"
#include "scloudstorage_p.h"

SCloudStorage::Private::Private(const QString &cloudName)
    : mCloudName(cloudName)
{
    
}

SCloudStorage::Private::~Private()
{
    qDeleteAll(mItems);
    mItems.clear();
    mItemsHash.clear();
}

QDataStream &operator<<(QDataStream &out, const SCloudItem &item)
{
    sDebug() << "Persisting item " << item.mUuid << item.mFields;
    out << item.mUuid << item.mFields;
    return out;
}

QDataStream &operator>>(QDataStream &in, SCloudItem &item)
{
    QString uuid;
    QHash<QString, QVariant> fields;

    in >> uuid >> fields;

    item.mUuid = uuid;
    item.mFields = fields;

    sDebug() << "Loaded " << uuid << fields;
    //return in >> item.mUuid >> item.mFields;
    return in;
}

QDebug operator<<(QDebug dbg, const SCloudItem &item)
{
    dbg.nospace() << item.mUuid << "(" << item.mFields << ")";
    return dbg.space();
}
