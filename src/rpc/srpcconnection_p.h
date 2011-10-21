/*
 * Copyright (C) 2011 Robin Burchell
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

#ifndef SRPCCONNECTION_P_H
#define SRPCCONNECTION_P_H

// Qt
#include <QObject>
#include <QString>
#include <QHostInfo>

// Us
#include "srpcsocket_p.h"
#include "srpcconnection.h"
#include "bonjourrecord.h"
#include "bonjourserviceresolver.h"

class SRpcConnectionPrivate : public QObject
{
    Q_OBJECT
public:
    explicit SRpcConnectionPrivate(const QString &interfaceName, QObject *parent);
    virtual ~SRpcConnectionPrivate();

    void invoke(const QString &command, const QVariantHash &parameters);
    void listen(const QString &signal, QObject *object, const char *slot);

private slots:
    void onConnected();
    void onDisconnected();
    void connectToServer(const QHostInfo &hostInfo, int);
    void updateRecords(const QList<BonjourRecord> &list);
    void processData(QByteArray data);

private:
    QString mInterfaceName;
    SRpcSocket mSocket;
    QList<QByteArray> mPendingMessages;
    BonjourServiceResolver mResolver;
};

#endif // SRPCCONNECTION_H
