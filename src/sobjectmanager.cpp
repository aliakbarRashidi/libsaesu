/*
 * Copyright (C) 2011 Robin Burchell <robin.burchell@collabora.co.uk>
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
#include <QSqlQuery>
#include <QStringList>

// Us
#include "sobjectmanager.h"
#include "sobjectmanager_p.h"
#include "sabstractobjectrequest.h"

// TODO: move to a seperate .cpp
SObjectManager::Private::Private(const QString &tableName, QObject *parent)
    : QObject(parent)
    , mTableName(tableName)
{
}

QSqlDatabase SObjectManager::Private::connection()
{
    if (!mConnection.isValid()) {
        mConnection = QSqlDatabase::addDatabase("QSQLITE", "cloud://saesu" + mTableName);
        mConnection.setDatabaseName(mTableName);
        if (!mConnection.open()) {
            // TODO: error handling
            sWarning() << "Couldn't open database";
            return mConnection;
        }

        // TODO: make this work with multiple ObjectManagers
        if (!mConnection.tables().contains("_saesu")) {
            QSqlQuery q(mConnection);
            mConnection.transaction();

            // create table(s)
            sDebug() << "Creating tables";

            q.exec("CREATE TABLE _saesu (version integer)");
            q.exec("INSERT INTO _saesu VALUES (1)");

            q.exec("CREATE TABLE objects (key primary key, object blob)");

            mConnection.commit();
        }
    }

    return mConnection;
}

SObjectManager::SObjectManager(const QString &tableName, QObject *parent)
    : QObject(parent)
    , d(new Private(tableName, this))
{
}

SObjectManager::~SObjectManager()
{
}

