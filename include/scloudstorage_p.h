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

// Us
#include "scloudstorage.h"
class SIpcChannel;
struct SCloudItem
{
    // TODO: make hash look up a SCloudItem type, SCloudItem should contain field timestamps + hashes
    QString mUuid;
    QHash<QString, QVariant> mFields;
};

QDataStream &operator<<(QDataStream &out, const SCloudItem &item);
QDataStream &operator>>(QDataStream &in, SCloudItem &item);
QDebug operator<<(QDebug dbg, const SCloudItem &item3);

class SCloudStorage::Private : public QObject
{
    Q_OBJECT
public:
    Private(SCloudStorage *parent, const QString &cloudName);
    virtual ~Private();

    SCloudStorage *q;
    QString mCloudName;
    QList<SCloudItem *> mItems;
    QHash<QString, SCloudItem *> mItemsHash;
    SIpcChannel *mLocalIpcChannel;
    bool mProcessingIpc;

    void insertItem(SCloudItem *item);
    void removeItem(SCloudItem *item);

signals:
    void created(const QString &uuid);
    void destroyed(const QString &uuid);

private slots:
    void onLocalIpcMessage(const QString &message, const QByteArray &data);
    void doSendLocalCreated(const QString &uuid);
    void doSendLocalDestroyed(const QString &uuid);
    void doSendLocalChanged(const QString &uuid, const QString &fieldName);
};

#endif // SCLOUDSTORAGE_P_H
