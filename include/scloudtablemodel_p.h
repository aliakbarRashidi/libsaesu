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

#ifndef SCLOUDTABLEMODEL_P_H
#define SCLOUDTABLEMODEL_P_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QUuid>

// Us
#include "scloudtablemodel.h"
class SCloudItem;

class SCloudTableModel::Private : public QObject
{
    Q_OBJECT
public:
    Private(SCloudTableModel *parent, SCloudStorage *cloud);

    SCloudTableModel *q;
    SCloudStorage *mCloud;
    QList<QString> mColumnNames;
    QList<QUuid> mRows;

public slots:
    void onItemCreated(const QUuid &uuid);
    void onItemDestroyed(const QUuid &uuid);
    void onItemChanged(const QUuid &uuid, const QString &fieldName);

signals:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
};

#endif // SCLOUDTABLEMODEL_P_H
