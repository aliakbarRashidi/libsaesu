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

#include "sdbmdatabase.h"
#include "sdbmdatabase_p.h"

SDBMDatabase::SDBMDatabase(const QString &databaseDirPath)
    : d(new Private(databaseDirPath))
{
}

/*!
 * Checks whether or not the database contains the data identified by the given \a key.
 *
 * Returns true if the key exists.
 */
bool SDBMDatabase::hasItem(const QByteArray &key)
{
    return d->hasItem(key);
}

/*!
 * Removes the value identified by the given \a key from the database.
 */
void SDBMDatabase::remove(const QByteArray &key)
{
    d->remove(key);
}

/*!
 * Sets the data identified by the given \a key to be \a value.
 */
void SDBMDatabase::set(const QByteArray &key, const QByteArray &value)
{
    d->set(key, value);
}

/*!
 * Retrieves the data associated by the given \a key.
 */
QByteArray SDBMDatabase::get(const QByteArray &key, bool *ok)
{
    return d->get(key, ok);
}

/*!
 * Retrieves a list of the keys used in this database.
 */
const QList<QByteArray> &SDBMDatabase::keys() const
{
    return d->keys();
}
