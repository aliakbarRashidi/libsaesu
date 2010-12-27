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

#ifndef SCLOUDSTORAGE_P_H
#define SCLOUDSTORAGE_P_H

// Qt
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QVariant>

struct SCloudItem
{
    // TODO: make hash look up a SCloudItem type, SCloudItem should contain field timestamps + hashes
    QString mUuid;
    QHash<QString, QVariant> mFields;
};

QDataStream &operator<<(QDataStream &out, const SCloudItem &item);
QDataStream &operator>>(QDataStream &in, SCloudItem &item);
QDebug operator<<(QDebug dbg, const SCloudItem &item);

class SCloudStorage::Private
{
public:
    Private(const QString &cloudName);
    virtual ~Private();

    QString mCloudName;

    QList<SCloudItem *> mItems;
    QHash<QString, SCloudItem *> mItemsHash;
};

#endif // SCLOUDSTORAGE_P_H
