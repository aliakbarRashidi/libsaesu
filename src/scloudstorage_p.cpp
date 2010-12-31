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
#include <QtGui/QDesktopServices>
#include <QtCore/QCoreApplication>

// Us
#include "sdbmdatabase.h"
#include "scloudstorage.h"
#include "scloudstorage_p.h"
#include "sipcchannel.h"

static QString s_cloudPath(const QString &cloudName)
{
    QCoreApplication *a = QCoreApplication::instance();

    QString orgName = a->organizationName();
    QString appName = a->applicationName();

    a->setOrganizationName(QLatin1String("saesu"));
    a->setApplicationName(QLatin1String("clouds"));

    QString cloudPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

    a->setOrganizationName(orgName);
    a->setApplicationName(appName);

//    QDir d;
//    d.mkpath(cloudPath);

    return cloudPath + QLatin1Char('/') + cloudName;
}

SCloudStorage::Private::Private(SCloudStorage *parent, const QString &cloudName)
    : QObject(parent)
    , q(parent)
    , mCloudName(cloudName)
    , mDatabase(s_cloudPath(cloudName))
    , mLocalIpcChannel(new SIpcChannel("cloud://" + cloudName, this))
    , mProcessingIpc(false)
{
    connect(mLocalIpcChannel, SIGNAL(received(QString,QByteArray)), this, SLOT(onLocalIpcMessage(QString,QByteArray)));
    connect(q, SIGNAL(created(QByteArray)), this, SLOT(doSendLocalCreated(QByteArray)));
    connect(q, SIGNAL(destroyed(QByteArray)), this, SLOT(doSendLocalDestroyed(QByteArray)));
    connect(q, SIGNAL(changed(QByteArray, QString)), this, SLOT(doSendLocalChanged(QByteArray, QString)));
}

SCloudStorage::Private::~Private()
{
}

void SCloudStorage::Private::onLocalIpcMessage(const QString &message, const QByteArray &data)
{
    mProcessingIpc = true;

    QDataStream stream(data);
    QByteArray uuid;
    stream >> uuid;

    if (message == "created(QByteArray)") {
        sDebug() << "Informed about creation of " << uuid.toHex();
        emit created(uuid);
    } else if (message == "destroyed(QByteArray") {
        sDebug() << "Informed about destruction of " << uuid.toHex();
        emit destroyed(uuid);
    } else if (message == "changed(QByteArray)") {
        QString fieldName;

        stream >> fieldName;
        emit changed(uuid, fieldName);
        sDebug() << "Read change for " << uuid.toHex() << " to field " << fieldName;
    }

    mProcessingIpc = false;
}

void SCloudStorage::Private::doSendLocalCreated(const QByteArray &uuid)
{
    // prevent endless loop nightmares
    if (mProcessingIpc)
        return;

    // let others know about the creation
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << uuid;

    sDebug() << "Notifying about creation of " << uuid.toHex();
    mLocalIpcChannel->sendMessage("created(QByteArray)", data);
}

void SCloudStorage::Private::doSendLocalDestroyed(const QByteArray &uuid)
{
    // prevent endless loop nightmares
    if (mProcessingIpc)
        return;

    // let others know about the destruction
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << uuid;

    sDebug() << "Notifying about destruction of " << uuid.toHex();
    mLocalIpcChannel->sendMessage("destroyed(QByteArray)", data);
}

void SCloudStorage::Private::doSendLocalChanged(const QByteArray &uuid, const QString &fieldName)
{
    // prevent endless loop nightmares
    if (mProcessingIpc)
        return;

    // let others know about the change
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    fetchItem(uuid);
    stream << uuid;
    stream << fieldName;
    stream << mCurrentItem.mFields[fieldName];

    sDebug() << "Notifying about change of " << fieldName << " on " << uuid.toHex();
    mLocalIpcChannel->sendMessage("changed(QByteArray)", data);
}

void SCloudStorage::Private::saveItem(const QByteArray &uuid, SCloudItem *item)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << *item;

    bool itemCreated = false;
    if (!mDatabase.hasItem(uuid))
        itemCreated = true;

    mDatabase.set(uuid, data);

    if (itemCreated) {
        emit created(uuid);
        sDebug() << "Inserted " << uuid.toHex();
    }
}

void SCloudStorage::Private::removeItem(const QByteArray &uuid)
{
    mDatabase.remove(uuid);
    emit destroyed(uuid);
    sDebug() << "Removed " << uuid.toHex();
}

bool SCloudStorage::Private::fetchItem(const QByteArray &uuid)
{
    bool ok = false;
    QByteArray data = mDatabase.get(uuid, &ok);
    if (!ok)
        return false;

    QDataStream stream(data);

    stream >> mCurrentItem;
    return true;
}

QDataStream &operator<<(QDataStream &out, const SCloudItem &item)
{
    out << item.mTimeStamp;
    out << item.mHash;
    out << item.mFields;
    return out;
}

QDataStream &operator>>(QDataStream &in, SCloudItem &item)
{
    in >> item.mTimeStamp;
    in >> item.mHash;
    in >> item.mFields;

    return in;
}

QDebug operator<<(QDebug dbg, const SCloudItem &item)
{
    dbg.nospace() << "(" << item.mFields << ")";
    return dbg.space();
}
