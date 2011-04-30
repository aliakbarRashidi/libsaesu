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
#include <QVariant>

// Us
#include "sobjectmanager.h"
#include "sobjectmanager_p.h"
#include "sabstractobjectrequest.h"

// TODO: move to a seperate .cpp
SObjectManager::Private::Private(const QString &tableName, QObject *parent)
    : QObject(parent)
    , mTableName(tableName)
    , mIpcChannel(QLatin1String("saesu-cloud://") + tableName)
{
    connect(&mIpcChannel, SIGNAL(received(QString,QByteArray)), this, SLOT(onIpcMessage(QString,QByteArray)));
}

QSqlDatabase SObjectManager::Private::connection()
{
    if (!mConnection.isValid()) {
        mConnection = QSqlDatabase::addDatabase("QSQLITE", QLatin1String("saesu-cloud://") + mTableName);
        mConnection.setDatabaseName(mTableName);
        if (!mConnection.open()) {
            // TODO: error handling
            sWarning() << "Couldn't open database";
            return mConnection;
        }

        const int currentDbVersion = 3;

        // TODO: make this work with multiple ObjectManagers
        if (!mConnection.tables().contains("_saesu")) {
            QSqlQuery q(mConnection);
            mConnection.transaction();

            // create table(s)
            sDebug() << "Creating tables";

            q.exec("CREATE TABLE _saesu (version integer)");
            q.exec("INSERT INTO _saesu VALUES (" + QString::number(currentDbVersion) + ")");
            q.exec("CREATE TABLE objects (key primary key, timestamp integer, hash blob, object blob)");
            q.exec("CREATE TABLE deletelist (key primary key, timestamp integer)");

            mConnection.commit();
        } else {
            QSqlQuery q(mConnection);
            mConnection.transaction();

            sDebug() << "Checking for migration";
            q.exec("SELECT version FROM _saesu");
            q.next();
            qint64 dbVersion = q.value(0).toLongLong();
            
            switch (dbVersion) {
                case currentDbVersion:
                    sDebug() << "Database up to date";
                    break;
                case 1:
                case 2:
                    // need to add a 'deletelist' table.
                    q.exec("CREATE TABLE deletelist (key primary key, timestamp integer)");
                    sDebug() << "Migrated successfully from schema v1";
                    break;
                default:
                    qCritical("I don't understand schema version!");
                    
            }

            q.exec("UPDATE _saesu SET version = '" + QString::number(currentDbVersion) + "'");

            mConnection.commit();
        }
    }

    return mConnection;
}

void SObjectManager::Private::onIpcMessage(const QString &message, const QByteArray &data)
{
    QDataStream stream(data);
    QList<SObjectLocalId> objects;
    stream >> objects;
    SObjectManager *q = static_cast<SObjectManager*>(parent());

    if (message == "added(QList<SObjectLocalId>)") {
        emit q->objectsAdded(objects);
    } else if (message == "removed(QList<SObjectLocalId>)") {
        emit q->objectsRemoved(objects);
    } else if (message == "updated(QList<SObjectLocalId>)") {
        emit q->objectsUpdated(objects);
    }
}

SObjectManager::SObjectManager(const QString &tableName, QObject *parent)
    : QObject(parent)
    , d(new Private(tableName, this))
{
}

SObjectManager::~SObjectManager()
{
}

