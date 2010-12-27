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
#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>

// Us
#include "scloudstorage.h"
#include "scloudstorage_p.h"
#include "sclouditem.h"

/*!
    \class SCloudStorage
    \brief SCloudStorage offers storage and management of data.

    SCloudStorage provides management of SCloudItem instances, and is responsible
    for keepign SCloudItem instances under its control persisted to disk,
    as well as kept in sync across the network.
    
    Synchronisation and storage is controlled by cloud naming, e.g. a cloud named
    'contacts' will be replicated across multiple devices advertising the same
    cloud.

    \sa SCloudItem
*/

SCloudStorage::SCloudStorage(const QString &cloudName, QObject *parent)
    : QObject(parent)
    , d(new Private(cloudName))
{
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
    ptr->load();
    return ptr;
}

/*!
 * Registers a given \a type pointer as belonging to a \a className.
 *
 * This is needed as a hack to work around Qt's meta object system not allowing us
 * to look up a QMetaObject instance from a given class name, as SCloudStorage must
 * create arbitrary class instances from class name.
 */
const QMetaObject *SCloudStorage::registerType(const QString &className, const QMetaObject *type)
{
    static QHash<QString, const QMetaObject *> types;
    if (type) {
        // setting type
        types.insert(className, type);
        return type;
    }

    // the hidden magic here is that we can also return a pointer when asked
    // this behaviour is not documented, as it is only used internally.
    QHash<QString, const QMetaObject *>::ConstIterator it = types.find(className);

    if (it != types.end())
        return *it;

    return NULL;
}

/*!
 * \internal
 *
 * Tries to load this cloud instance from disk if possible.
 *
 * \note You should avoid calling this from third party code; SCloudStorage
 * initialises itself for you.
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
        QString className;
        stream >> className;
        
        QByteArray bytes;
        stream >> bytes;
        
        const QMetaObject *obj = registerType(className);
        if (S_VERIFY(obj, "unknown type! eek! did you forget to load a plugin?"))
            continue;

        SCloudItem *item = qobject_cast<SCloudItem *>(obj->newInstance(Q_ARG(QString, d->mCloudName)));
        if (S_VERIFY(item, "couldn't construct type!"))
            continue;

        item->deserialize(bytes, version);
        sDebug() << "Read object of type " << className << "(" << item->uuid() << ")";
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
    stream << (quint32)d->mItems.count(); // TODO: enough items?

    // XXX: this won't scale
    foreach (SCloudItem *item, d->mItems) {
        QString className = item->metaObject()->className();
        sDebug() << "Saving item of type " << className << "(" << item->uuid() << ")";
        stream << className;
        stream << item->serialize();
    }

    QFile f(d->mCloudName + "_cloud");
    f.open(QIODevice::WriteOnly);
    f.write(bytes);

    sDebug() << "Saved cloud: " << d->mCloudName;
}

/*!
 * \internal
 * Adds a given \a item to this cloud.
 *
 * \note SCloudItem should do this for you; you shouldn't have to use it.
 */
void SCloudStorage::addItem(SCloudItem *item)
{
    if (S_VERIFY(d->mItemsHash.find(item->uuid()) == d->mItemsHash.end(), "same item inserted twice"))
        return;

    item->setParent(this);
    d->mItemsHash.insert(item->uuid(), item);
    d->mItems.append(item);
    sDebug() << "Added " << item->uuid() << " to " << d->mCloudName;
}

/*!
 * \internal
 * Removes a given \a item from this cloud.
 *
 * \note You should probably rethink your design if you have to use this.
 */
void SCloudStorage::removeItem(SCloudItem *item)
{
    if (S_VERIFY(d->mItemsHash.find(item->uuid()) != d->mItemsHash.end(), "removing an item that was never there"))
        return;

    item->setParent(0);
    d->mItemsHash.remove(item->uuid());
    d->mItems.removeAll(item);
    sDebug() << "Removed " << item->uuid() << " from " << d->mCloudName;
}

/*!
 * Retrieves an item with a given \a uuid from this cloud.
 */
SCloudItem *SCloudStorage::item(const QString &uuid) const
{
    QHash<QString, SCloudItem *>::ConstIterator it = d->mItemsHash.find(uuid);
   
    if (it == d->mItemsHash.end())
        return NULL;

    return *it;
}

/*!
 * \internal
 *
 * Returns a list of all items in this cloud.
 */
QList<SCloudItem *> SCloudStorage::items() const
{
   return d->mItems;
}
