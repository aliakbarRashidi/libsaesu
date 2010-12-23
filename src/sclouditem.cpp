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

// Us
#include "sclouditem.h"
#include "sclouditem_p.h"
#include "scloudstorage.h"

/*!
 * Constructs an SCloudInstance belonging in the cloud named \a cloudName.
 */
SCloudItem::SCloudItem(const QString &cloudName)
    : QObject()
    , d(new Private(cloudName, this))
{
    SCloudStorage *storage = SCloudStorage::instance(cloudName);
    storage->addItem(this);
}

SCloudItem::~SCloudItem()
{
    delete d;
}

/*!
 * Retrieves the unique identifier of this item.
 */
const QString &SCloudItem::uuid() const
{
    return d->mUuid;
}

/*!
 * Retrieves the last modification time of this item.
 */
const QDateTime &SCloudItem::lastModified() const
{
    return d->mLastModified;
}

/*!
 * Retrieves the object hash of this item.
 */
const QByteArray &SCloudItem::hash() const
{
    return d->mHash;
}

/*!
 * Marks this object as modified. This will update the last modification timestamp
 * and the data hash.
 *
 * You should call this whenever you modify a Q_PROPERTY on your SCloudItem instance.
 */
void SCloudItem::objectModified()
{
    d->objectModified();
}

/*!
 * Serialises all properties of this cloud item to be written to disk, returning
 * a byte representation of the item.
 */
QByteArray SCloudItem::serialize() const
{
    return d->serialize();
}

/*!
 * Deserialises all of the properties of this cloud item from a given array of \a bytes.
 *
 * An optional \a version parameter is provided which may be used to change deserialisation
 * behaviour in an unspecified way.
 *
 * TODO: define this
 */
void SCloudItem::deserialize(QByteArray &bytes, int version)
{
    if (S_VERIFY(version == 0, "I don't know how to deserialise this version!'"))
        return;

    d->deserialize(bytes, version);
}

/*!
 * Sets the UUID of this cloud item to \a uuid, including keeping the associated
 * cloud store up to date.
 */
void SCloudItem::setUuid(const QString &uuid)
{
    // update the hash
    SCloudStorage *storage = SCloudStorage::instance(d->mCloudName);
    storage->removeItem(this);

    d->mUuid = uuid;
    
    storage->addItem(this);
}

/*!
 * Sets the last modified time of this cloud item to \a datetime.
 */
void SCloudItem::setLastModified(const QDateTime &datetime)
{
    d->mLastModified = datetime;
}

/*!
 * Sets the object hash of this cloud item.
 */
void SCloudItem::setHash(const QByteArray &hash)
{
    d->mHash = hash;
}

