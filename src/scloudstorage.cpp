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
#include <QtCore/QDataStream>
#include <QtCore/QFile>

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
    , d(new Private(cloudName))
{
    load();
}

SCloudStorage::~SCloudStorage()
{
    save();
    delete d;
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
 * \internal
 *
 * Tries to load this cloud instance from disk if possible.
 *
 * \note You should not need to use this in userspace code, as SCloudStorage's constructor tries to load for you.
 */
void SCloudStorage::load()
{
    qDeleteAll(d->mItems);
    d->mItems.clear();

    QFile f(d->mCloudName + "_cloud");
    f.open(QIODevice::ReadOnly);
    QDataStream stream(&f); // TODO: is this safe?

    // no file
    if (!f.size())
        return;

    quint32 magic;
    stream >> magic;

    // read our magical header and make sure it matches, if it doesn't,
    // don't touch file with a bargepole, we'll overwrite it later
    if (S_VERIFY(magic == 0xDEAAFFEB, "magic is different!"))
        return;

    quint8 version;
    stream >> version;
    sDebug() << "Parsing stream version " << version;

    quint32 itemsCount;
    stream >> itemsCount;
    sDebug() << "Reading " << itemsCount << " items into cloud " << d->mCloudName;

    for (quint32 i = 0; i < itemsCount; ++i) {
        SCloudItem *item = new SCloudItem;
        stream >> *item;
        sDebug() << *item;
        d->mItems.append(item);
        d->mItemsHash.insert(item->mUuid, item);
    }
}

/*!
 * Tries to save this cloud to disk, including serialising all objects belonging
 * to it.
 *
 * \note This is an expensive operation; it should not be performed often.
 */
void SCloudStorage::save()
{
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);

    stream << (quint32)0xDEAAFFEB; // header
    stream << (quint8)0; // version
    stream << (quint32)d->mItems.count();

    foreach (SCloudItem *item, d->mItems)
        stream << *item;

    QFile f(d->mCloudName + "_cloud");
    f.open(QIODevice::WriteOnly);
    f.write(bytes);

    sDebug() << "Saved cloud: " << d->mCloudName;
}

/*!
 * Retrieves a given \a field from a \a UUID.
 */
QVariant SCloudStorage::get(const QString &uuid, const QString &field) const
{
    if (S_VERIFY(d->mItemsHash.find(uuid) != d->mItemsHash.end(), "couldn't find item"))
        return QVariant();

    SCloudItem *item = *(d->mItemsHash.find(uuid));
    return *(item->mFields.find(field));
}

/*!
 * Sets a \a field to \a data on a given \a uuid.
 */
void SCloudStorage::set(const QString &uuid, const QString &field, const QVariant &data)
{
    if (S_VERIFY(d->mItemsHash.find(uuid) != d->mItemsHash.end(), "couldn't find item"))
        return;

    SCloudItem *item = *(d->mItemsHash.find(uuid));
    item->mFields[field] = data;

    sDebug() << "Changed " << uuid << " field: " << field << " to " << data;
    emit changed(uuid);
}

/*!
 * Creates a new item of data with a unique identifier.
 * Returns the unique identifier for this data.
 */
QString SCloudStorage::create(const QHash<QString, QVariant> &fields)
{
    SCloudItem *item = new SCloudItem;

    // hehe.. let's hope it isn't forever!
    int tries = 0;
    forever {
        item->mUuid = QUuid::createUuid().toString();
        item->mUuid= item->mUuid.mid(1, item->mUuid.length() - 2);

        if (d->mItemsHash.find(item->mUuid) == d->mItemsHash.end())
            break;

        // paranoia
        if (tries++ == 100)
            sDebug() << "Generating a unique ID has taken way too many attempts; broken UUID generator?";

        // more paranoia
        if (tries == 1000)
            qCritical("SCloudStorage: either a morbidly large cloud or a broken UUID generator, panic!");
    }

    d->mItems.append(item);
    d->mItemsHash.insert(item->mUuid, item);

    sDebug() << "Created " << item->mUuid << "(" << fields << ")";
    emit created(item->mUuid);
    return item->mUuid;
}

/*!
 * Destroys the data associated with a given \a uuid, removing it from the cloud.
 */
void SCloudStorage::destroy(const QString &uuid)
{
    if (S_VERIFY(d->mItemsHash.find(uuid) != d->mItemsHash.end(), "couldn't find item"))
        return;

    SCloudItem *item = *(d->mItemsHash.find(uuid));
    d->mItems.removeAll(item);
    d->mItemsHash.remove(uuid);
    delete item;

    sDebug() << "Destroyed " << uuid;
    emit destroyed(uuid);
}

/*!
 * \internal
 *
 * Retrieves a list of all items in this cloud.
 */
QList<SCloudItem *> SCloudStorage::items() const
{
    return d->mItems;
}
