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
#include "sipcchannel.h"

SCloudStorage::Private::Private(SCloudStorage *parent, const QString &cloudName)
    : QObject(parent)
    , q(parent)
    , mCloudName(cloudName)
    , mLocalIpcChannel(new SIpcChannel("cloud://" + cloudName, this))
    , mProcessingIpc(false)
{
    connect(mLocalIpcChannel, SIGNAL(received(QString,QByteArray)), this, SLOT(onLocalIpcMessage(QString,QByteArray)));
    connect(q, SIGNAL(created(QString)), this, SLOT(doSendLocalCreated(QString)));
    connect(q, SIGNAL(destroyed(QString)), this, SLOT(doSendLocalDestroyed(QString)));
    connect(q, SIGNAL(changed(QString, QString)), this, SLOT(doSendLocalChanged(QString, QString)));
}

SCloudStorage::Private::~Private()
{
    qDeleteAll(mItems);
    mItems.clear();
    mItemsHash.clear();
}

void SCloudStorage::Private::onLocalIpcMessage(const QString &message, const QByteArray &data)
{
    mProcessingIpc = true;

    QDataStream stream(data);
    QString uuid;
    stream >> uuid;

    if (message == "created(QString)") {
        sDebug() << "Informed about creation of " << uuid;
        SCloudItem *item = new SCloudItem;
        item->mUuid = uuid;
        insertItem(item);
    } else if (message == "destroyed(QString") {
        sDebug() << "Informed about destruction of " << uuid;
    } else if (message == "changed(QString)") {
        QString fieldName;
        QVariant value;

        stream >> fieldName;
        stream >> value;

        q->set(uuid, fieldName, value);
        sDebug() << "Read change for " << uuid << " to field " << fieldName;
    }

    mProcessingIpc = false;
}

void SCloudStorage::Private::doSendLocalCreated(const QString &uuid)
{
    // prevent endless loop nightmares
    if (mProcessingIpc)
        return;

    // let others know about the creation
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << uuid;

    sDebug() << "Notifying about creation of " << uuid;
    mLocalIpcChannel->sendMessage("created(QString)", data);
}

void SCloudStorage::Private::doSendLocalDestroyed(const QString &uuid)
{
    // prevent endless loop nightmares
    if (mProcessingIpc)
        return;

    // let others know about the destruction
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << uuid;

    sDebug() << "Notifying about destruction of " << uuid;
    mLocalIpcChannel->sendMessage("destroyed(QString)", data);
}

void SCloudStorage::Private::doSendLocalChanged(const QString &uuid, const QString &fieldName)
{
    // prevent endless loop nightmares
    if (mProcessingIpc)
        return;

    // let others know about the change
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    SCloudItem *item = (*mItemsHash.find(uuid));
    stream << uuid;
    stream << fieldName;
    stream << item->mFields[fieldName];

    sDebug() << "Notifying about change of " << fieldName << " on " << uuid;
    mLocalIpcChannel->sendMessage("changed(QString)", data);
}

void SCloudStorage::Private::insertItem(SCloudItem *item)
{
    mItems.append(item);
    mItemsHash.insert(item->mUuid, item);
    emit created(item->mUuid);
    sDebug() << "Inserted " << item->mUuid;
}

void SCloudStorage::Private::removeItem(SCloudItem *item)
{
    mItems.removeAll(item);
    mItemsHash.remove(item->mUuid);
    emit destroyed(item->mUuid);
    sDebug() << "Removed " << item->mUuid;
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
