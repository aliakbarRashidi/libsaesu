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
