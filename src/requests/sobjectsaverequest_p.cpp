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

// Us
#include "sobjectsaverequest.h"
#include "sobjectsaverequest_p.h"
#include "sobject.h"
#include "sobjectmanager.h"
#include "sobjectmanager_p.h"

SObjectSaveRequest::Private::Private(QObject *parent)
    : SAbstractObjectRequest::Private(parent)
{
}

void SObjectSaveRequest::Private::start(SObjectManager *manager)
{
    QSqlDatabase db = manager->d->connection();
    QSqlQuery q(db);

    foreach (SObject obj, mObjects) {
        if (obj.uuid().isNull()) {
            obj.setUuid(QUuid::createUuid());

            // insert
            sDebug() << "Inserting " << obj.uuid();
        } else {
            sDebug() << "Updating " << obj.uuid();
        }

        q.prepare("INSERT OR REPLACE INTO objects (key, object) VALUES (:key, :data)");
        q.bindValue(":key", obj.uuid().toString());
        
        QByteArray buf;
        QDataStream ds(&buf, QIODevice::ReadWrite);
        ds << obj;
        
        q.bindValue(":data", buf);
        q.exec();

        sDebug() << "Serialised form " << buf.toHex();
        sDebug() << "No hex: " << buf;
    }

    db.commit();

    //emit finished(this);
    sDebug() << "Done";
    deleteLater();
}
