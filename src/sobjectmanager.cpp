/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, and all associated material(s), including but not limited
 * to documentation are protected by copyright. All rights reserved.
 * Copying, including reproducing, storing, adapting, translating, or
 * reverse-engineering any or all of this material requires prior written
 * consent. This material also contains confidential information which
 * may not be disclosed in any form without prior written consent.
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
    , mIpcChannel(QLatin1String("saesu-cloud://") + tableName)
{
    connect(&mIpcChannel, SIGNAL(received(QString,QByteArray)), this, SLOT(onIpcMessage(QString,QByteArray)));
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

