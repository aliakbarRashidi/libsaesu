/*
 * Copyright (C) 2010-2011 Robin Burchell
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Qt
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
#include <QCoreApplication>
#include <QDir>
#include <QDesktopServices>

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
        QString databasePath;
        QCoreApplication *a = QCoreApplication::instance();

        QString orgName = a->organizationName();
        QString appName = a->applicationName();

        a->setOrganizationName(QLatin1String("saesu"));
        a->setApplicationName(QLatin1String("clouds"));

        databasePath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

        QDir dirPath(databasePath);
        if (!dirPath.exists())
            dirPath.mkpath(databasePath);


        // restore app name/org details
        a->setOrganizationName(orgName);
        a->setApplicationName(appName);

        mConnection = QSqlDatabase::addDatabase("QSQLITE", QLatin1String("saesu-cloud://") + mTableName);
        mConnection.setDatabaseName(databasePath + "/" + mTableName);
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

