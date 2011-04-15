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
    , d(new Private(this, cloud))
{
    connect(cloud, SIGNAL(created(QByteArray)), d, SLOT(onItemCreated(QByteArray)));
    connect(cloud, SIGNAL(destroyed(QByteArray)), d, SLOT(onItemDestroyed(QByteArray)));
    connect(cloud, SIGNAL(changed(QByteArray)), d, SLOT(onItemChanged(QByteArray)));
    connect(d, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SIGNAL(dataChanged(QModelIndex,QModelIndex)));
}

void SCloudTableModel::setColumns(const QList<QString> &columns)
{
    // technically, might not be necessary to do a full reset, but this is easier
    beginResetModel();
    d->mColumnNames.clear();
    d->mColumnNames.append("UUID");
    d->mColumnNames.append(columns);
    endResetModel();
}

QModelIndex SCloudTableModel::index(int row, int column, const QModelIndex &parent) const
{
    // table, not a tree.
    if (parent != QModelIndex())
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex SCloudTableModel::parent(const QModelIndex &index) const
{
    // table, not a tree
    return QModelIndex();
}

int SCloudTableModel::rowCount(const QModelIndex &parent) const
{
    // table, not a tree
    if (parent != QModelIndex())
        return 0;

    return d->mRows.count();
}

int SCloudTableModel::columnCount(const QModelIndex &parent) const
{
    // table, not a tree
    if (parent != QModelIndex())
        return 0;

    return d->mColumnNames.count();
}

QVariant SCloudTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    if (index.row() < 0 || index.row() >= d->mRows.count())
        return QVariant();

    if (index.column() < 0 || index.column() >= d->mColumnNames.count())
        return QVariant();

    if (index.column() == 0) {
        // special case: return uuid
        return d->mRows[index.row()].toHex();
    } else {
        const QString &propName = d->mColumnNames.at(index.column());

        return d->mCloud->get(d->mRows[index.row()], propName);
    }
}

bool SCloudTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // no editing the uuid, thanks
    if (index.column() == 0)
        return false;

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return false; // huh?

    if (index.row() < 0 || index.row() >= d->mRows.count())
        return false;

    if (index.column() < 0 || index.column() >= d->mColumnNames.count())
        return false;

    const QString &propName = d->mColumnNames.at(index.column());
    d->mCloud->set(d->mRows[index.row()], propName, value);
    return true;
}

Qt::ItemFlags  SCloudTableModel::flags(const QModelIndex &index) const
{
    if (index.row() < 0 || index.row() >= d->mRows.count())
        return 0;

    if (index.column() < 0 || index.column() >= d->mColumnNames.count())
        return 0;

    if (index.column() == 0)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QVariant SCloudTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    if (section < 0 || section >= d->mColumnNames.count())
        return QVariant();

    return d->mColumnNames.at(section);
}

bool SCloudTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    // table, not a tree
    if (parent != QModelIndex())
        return false;

    if (row < 0 || row >= d->mRows.count())
        return false;

    // cap number of rows to try remove
    if (row + count >= d->mRows.size())
        count = d->mRows.size() - row;

    // beginRemoveRows / endRemoveRows is covered in the item destruction signal
    // remove 'row' count times
    sDebug() << "Destroying " << row << count << " times";
    while (count > 0) {
        // data copy here *IS REQUIRED*.
        // otherwise, we pass a reference to mRows[row] around all the way until we get to
        // the destroy callback, which removes the row from mRows, which will cause a lot of pain.
        QByteArray key = d->mRows[row];
        sDebug() << "Destroying " << d->mRows[row].toHex();
        d->mCloud->destroy(key);
        count--;
    }

    return true;
}

/*!
 * Creates a row in the cloud, for population with data.
 */
bool SCloudTableModel::createRow()
{
    d->mCloud->create();
}
