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

#ifndef SOBJECTMANAGER_P_H
#define SOBJECTMANAGER_P_H

// Qt
#include <QObject>
#include <QString>
#include <QSqlDatabase>

// Us
#include "sobjectmanager.h"
#include "sipcchannel.h"

class SObjectManager::Private : public QObject
{
    Q_OBJECT
public:
    explicit Private(const QString &tableName, QObject *parent = 0);
    
    QSqlDatabase connection();

    QSqlDatabase mConnection;

    QString mTableName;

    SIpcChannel mIpcChannel;

private slots:
    void onIpcMessage(const QString &message, const QByteArray &data);
};

#endif // SOBJECTMANAGER_P_H
