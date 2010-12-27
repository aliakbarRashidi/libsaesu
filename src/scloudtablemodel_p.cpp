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
    foreach (const QString &uuid, mCloud->itemUUIDs())
        mRows.append(uuid);
}

void SCloudTableModel::Private::onItemCreated(const QString &uuid)
{
    sDebug() << "Item inserted: " << uuid;
    q->beginInsertRows(QModelIndex(), mRows.count(), mRows.count());
    mRows.append(uuid);
    q->endInsertRows();
}

void SCloudTableModel::Private::onItemDestroyed(const QString &uuid)
{
    int rowNumber = 0;

    foreach (const QString &rowuuid, mRows) {
        if (rowuuid == uuid)
            break;

        rowNumber++;
    }

    q->beginRemoveRows(QModelIndex(), rowNumber, rowNumber);
    mRows.removeAt(rowNumber);
    q->endRemoveRows();
}

void SCloudTableModel::Private::onItemChanged(const QString &uuid, const QString &fieldName)
{
    // find the coords
    int rowNumber = 0;
    int colNumber = 0;

    foreach (const QString &rowuuid, mRows) {
        if (rowuuid == uuid)
            break;

        rowNumber++;
    }

    foreach (const QString &colName, mColumnNames) {
        if (colName == fieldName)
            break;

        colNumber++;
    }

    sDebug() << "Data changed on UUID " << uuid << " row number " << rowNumber << ":" << colNumber;
    emit dataChanged(q->index(rowNumber, colNumber, QModelIndex()),
                     q->index(rowNumber, colNumber, QModelIndex()));
}
