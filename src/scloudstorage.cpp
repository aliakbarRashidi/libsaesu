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
#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QtCore/QUuid>
#include <QtCore/QCoreApplication>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>

// Us
#include "scloudstorage.h"
#include "scloudstorage_p.h"

/*!
    \class SCloudStorage
    \brief SCloudStorage offers storage and management of data.

    SCloudStorage provides management, persistance and synchronisation of data
    across multiple processes on a single host, as well as the wider network.
    
    Synchronisation and storage is controlled by cloud naming, e.g. a cloud named
    'contacts' will be replicated across multiple devices advertising the same
    cloud.
*/

/*!
 * \internal
 *
 * \sa instance()
 */
SCloudStorage::SCloudStorage(const QString &cloudName, QObject *parent)
    : QObject(parent)
    , d(new Private(this, cloudName))
{
    connect(d, SIGNAL(created(QByteArray)), SIGNAL(created(QByteArray)));
    connect(d, SIGNAL(destroyed(QByteArray)), SIGNAL(destroyed(QByteArray)));
    connect(d, SIGNAL(changed(QByteArray,QString)), SIGNAL(changed(QByteArray,QString)));
}

SCloudStorage::~SCloudStorage()
{
}

/*! Retrieves a cloud storage instance matching the given \a cloudName.
 *
 * If the given \a cloudName does not exist, a new cloud is created and returned,
 * otherwise the already existing cloud matching \a cloudName is returned.
 */
SCloudStorage *SCloudStorage::instance(const QString &cloudName)
{
    static QHash<QString, SCloudStorage *> hash;
    QHash<QString, SCloudStorage *>::ConstIterator it;

    if ((it = hash.find(cloudName)) != hash.end())
        return *it;

    SCloudStorage *ptr = new SCloudStorage(cloudName);
    hash.insert(cloudName, ptr);
    ptr->setParent(QCoreApplication::instance());
    return ptr;
}

/*!
 * Returns the name of this cloud
 */
const QString &SCloudStorage::cloudName() const
{
    return d->mCloudName;
}

/*!
 * Checks whether or not the cloud contains the item with the given \a uuid
 *
 * Returns true if the cloud contains this uuid.
 */
bool SCloudStorage::hasItem(const QByteArray &uuid) const
{
    if (!d->fetchItem(uuid))
        return false;

    return true;
}

/*!
 * Retrieves a given \a field from a \a UUID.
 */
QVariant SCloudStorage::get(const QByteArray &uuid, const QString &field) const
{
    if (S_VERIFY(hasItem(uuid), "couldn't find item"))
        return QVariant();

    d->fetchItem(uuid);
    return *(d->mCurrentItem.mFields.find(field));
}

/*!
 * Sets a \a field to \a data on a given \a uuid.
 */
void SCloudStorage::set(const QByteArray &uuid, const QString &field, const QVariant &data)
{
    if (S_VERIFY(hasItem(uuid), "couldn't find item"))
        return;

    d->fetchItem(uuid);
    d->mCurrentItem.mFields[field] = data;
    d->mCurrentItem.mTimeStamp = QDateTime::currentMSecsSinceEpoch();

    QCryptographicHash hasher(QCryptographicHash::Sha1);

    foreach (const QVariant &fieldData, d->mCurrentItem.mFields)
        hasher.addData(fieldData.toByteArray());

    d->mCurrentItem.mHash = hasher.result();

    sDebug() << "Changed " << uuid << " field: " << field << " to " << data;

    d->saveItem(uuid, &d->mCurrentItem);

    emit changed(uuid, field);
}

/*!
 * Creates a new item of data with a unique identifier.
 * Returns the unique identifier for this data.
 */
QByteArray SCloudStorage::create()
{
    SCloudItem item;
    QByteArray itemUuid;

    // hehe.. let's hope it isn't forever!
    int tries = 0;
    forever {
        QUuid uuid = QUuid::createUuid();
        QDataStream ds(&itemUuid, QIODevice::WriteOnly);
        ds << uuid;

        if (!hasItem(itemUuid))
            break;

        // paranoia
        if (tries++ == 100)
            sDebug() << "Generating a unique ID has taken way too many attempts; broken UUID generator?";

        // more paranoia
        if (tries == 1000)
            qCritical("SCloudStorage: either a morbidly large cloud or a broken UUID generator, panic!");
    }

    d->saveItem(itemUuid, &item);
    return itemUuid;
}

/*!
 * Destroys the data associated with a given \a uuid, removing it from the cloud.
 */
void SCloudStorage::destroy(const QByteArray &uuid)
{
    if (S_VERIFY(hasItem(uuid), "couldn't find item"))
        return;

    d->removeItem(uuid);
}

/*!
 * \internal
 *
 * Retrieves a list of all item UUIDs
 */
QList<QByteArray> SCloudStorage::itemUUIDs() const
{
    return d->mDatabase.keys();
}

/*!
 * \internal
 *
 * Retrieves the item associated with a given UUID.
 * This is used for synchronisation.
 */
SCloudItem *SCloudStorage::item(const QByteArray &uuid) const
{
    d->fetchItem(uuid);
    return &d->mCurrentItem;
}

/*!
 * \internal
 *
 * Adds a given item to the cloud.
 * This is used for synchronisation.
 */
void SCloudStorage::insertItem(const QByteArray &uuid, SCloudItem *item)
{
    d->saveItem(uuid, item);
}
