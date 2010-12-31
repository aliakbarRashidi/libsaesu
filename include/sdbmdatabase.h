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

#ifndef SDBMDATABASE_H
#define SDBMDATABASE_H

// Qt
#include <QByteArray>

// Us
#include "sglobal.h"

class SDBMDatabase
{
public:
    SDBMDatabase(const QString &databaseDirPath);

    bool hasItem(const QByteArray &key);
    void remove(const QByteArray &key);
    void set(const QByteArray &key, const QByteArray &value);
    QByteArray get(const QByteArray &key, bool *ok);
    QList<QByteArray> keys() const;

private:
    class Private;
    Private *d;
};

#endif // SDBMDATABASE_H
