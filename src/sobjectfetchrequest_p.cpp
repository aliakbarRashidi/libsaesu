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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>

#include "sobjectfetchrequest.h"
#include "sobjectfetchrequest_p.h"

SObjectFetchRequest::Private::Private(QObject *parent)
    : SAbstractObjectRequest::Private(parent)
{
    sDebug() << "Constructed";
}

void SObjectFetchRequest::Private::run()
{
    // TODO: needlessly reopening the connection sucks
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "saesu");
    db.setDatabaseName("saesu");
    bool ok = db.open();
    if (!db.open()) {
        // TODO: error handling
        sWarning() << "Couldn't open database";
        deleteLater();
        return;
    }

    if (!db.tables().contains("_saesu")) {
        sWarning() << "Table not found";
        deleteLater();
        return;
    }
    
    QSqlQuery q(db);
    q.exec("SELECT object FROM objects");
    
    while (q.next()) {
        QByteArray b(q.value(0).toByteArray());
        QDataStream ds(&b, QIODevice::ReadOnly);
        SObject obj;
        ds >> obj;
        mObjects << obj;

        sDebug() << "Read " << obj.uuid();
    }

    sDebug() << "Done";
    deleteLater();
}

