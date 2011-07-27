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

#ifndef SIPCCONNECTION_H
#define SIPCCONNECTION_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

// Us
#include "sglobal.h"
#include "sipcchannel.h"
class SIpcServer;

class SIpcConnection : public QLocalSocket
{
    Q_OBJECT

public:
    SIpcConnection(QObject *parent = 0);
    SIpcServer *mServerInstance;
    
    void setChannelName(const QString &channelName);
    const QString &channelName() const;
    
    void sendMessage(const QString &message, const QByteArray &data);

    bool isServer() const;

signals:
    void channelNameSet(const QString &channelName);
    void messageArrived(const QString &message, const QByteArray &data);
    void becameServer();
    
public slots:
    void reconnect();
    
private slots:
    void onReadyRead();
    void processData(const QByteArray &bytes);
    
private:    
    quint32 mBytesExpected;
    QByteArray mBuffer;
    QString mChannelName;
};

#endif // SIPCCONNECTION_H
