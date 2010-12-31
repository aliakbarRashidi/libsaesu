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
#include <QtCore/QUuid>

// Us
#include "scloudstorage.h"
#include "sdbmdatabase.h"
class SIpcChannel;

class SCloudStorage::Private : public QObject
{
    Q_OBJECT
public:
    Private(SCloudStorage *parent, const QString &cloudName);
    virtual ~Private();

    SCloudStorage *q;
    QString mCloudName;
    SDBMDatabase mDatabase;
    SIpcChannel *mLocalIpcChannel;
    bool mProcessingIpc;

    SCloudItem mCurrentItem;

    bool fetchItem(const QByteArray &uuid);
    void saveItem(const QByteArray &key, SCloudItem *item);
    void removeItem(const QByteArray &uuid);

signals:
    void changed(const QByteArray &uuid, const QString &fieldName);
    void created(const QByteArray &uuid);
    void destroyed(const QByteArray &uuid);

private slots:
    void onLocalIpcMessage(const QString &message, const QByteArray &data);
    void doSendLocalCreated(const QByteArray &uuid);
    void doSendLocalDestroyed(const QByteArray &uuid);
    void doSendLocalChanged(const QByteArray &uuid, const QString &fieldName);
};

#endif // SCLOUDSTORAGE_P_H
