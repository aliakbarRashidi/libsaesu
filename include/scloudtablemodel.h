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

#ifndef SCLOUDTABLEMODEL_H
#define SCLOUDTABLEMODEL_H

// Qt
#include <QtCore/QAbstractItemModel>

// Us
class SCloudStorage;

class SCloudTableModel : public QAbstractItemModel
{
public:
    SCloudTableModel(SCloudStorage *cloud, QObject *parent = 0);
    virtual ~SCloudTableModel() {}

    void setColumns(const QList<QString> &columns);
    QModelIndex index(int, int, const QModelIndex&) const;
    QModelIndex parent(const QModelIndex&) const;
    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex&) const;
    QVariant data(const QModelIndex&, int) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool removeRows (int row, int count, const QModelIndex &parent = QModelIndex());
    bool createRow();

private:
    class Private;
    Private *d;
};

#endif // SCLOUDTABLEMODEL_H
