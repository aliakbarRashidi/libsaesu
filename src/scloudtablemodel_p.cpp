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

#include "scloudstorage_p.h"
#include "scloudtablemodel.h"
#include "scloudtablemodel_p.h"

SCloudTableModel::Private::Private(SCloudTableModel *parent, SCloudStorage *cloud)
    : QObject(parent)
    , q(parent)
    , mCloud(cloud)
{
    mRows = mCloud->itemUUIDs();
}

void SCloudTableModel::Private::onItemCreated(const QByteArray &uuid)
{
    sDebug() << "Item inserted: " << uuid.toHex();
    q->beginInsertRows(QModelIndex(), mRows.count(), mRows.count());
    mRows.append(uuid);
    q->endInsertRows();
}

void SCloudTableModel::Private::onItemDestroyed(const QByteArray &uuid)
{
    sDebug() << "Removing " << uuid.toHex();
    int rowNumber = 0;

    foreach (const QByteArray &rowuuid, mRows) {
        if (rowuuid == uuid)
            break;

        rowNumber++;
    }

    sDebug() << "Removing 1 " << uuid.toHex();
    sDebug() << "Removing row " << rowNumber;
    q->beginRemoveRows(QModelIndex(), rowNumber, rowNumber);
    sDebug() << "Removing 2 " << uuid.toHex();
    mRows.removeAt(rowNumber);
    sDebug() << "Removing 3 " << uuid.toHex();
    q->endRemoveRows();
    sDebug() << "Removing 4 " << uuid.toHex();
}

void SCloudTableModel::Private::onItemChanged(const QByteArray &uuid)
{
    // find the coords
    int rowNumber = 0;

    foreach (const QByteArray &rowuuid, mRows) {
        if (rowuuid == uuid)
            break;

        rowNumber++;
    }

    sDebug() << "Data changed on UUID " << uuid.toHex() << " row number " << rowNumber;
    emit dataChanged(q->index(rowNumber, 0, QModelIndex()),
                     q->index(rowNumber, mColumnNames.count(), QModelIndex()));
}
