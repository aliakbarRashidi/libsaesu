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
#include <QtCore/QAbstractItemModel>

// Us
#include "scloudtablemodel.h"
#include "scloudtablemodel_p.h"
#include "scloudstorage.h"
#include "scloudstorage_p.h"

SCloudTableModel::SCloudTableModel(SCloudStorage *cloud, QObject *parent)
    : QAbstractItemModel(parent)
    , d(new Private(cloud))
{
}

QModelIndex SCloudTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex SCloudTableModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int SCloudTableModel::rowCount(const QModelIndex&) const
{
    return d->mCloud->items().count( );
}

int SCloudTableModel::columnCount(const QModelIndex&) const
{
    return 1;
}

QVariant SCloudTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
        return d->mCloud->items()[index.row()]->mUuid;

    return QVariant();
}
