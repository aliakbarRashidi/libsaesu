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

#ifndef SIPCCHANNEL_P_H
#define SIPCCHANNEL_P_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>

// Us
#include "sglobal.h"
#include "sipcchannel.h"
class SIpcConnection;

class SIpcChannel::Private : public QObject
{
    Q_OBJECT

public:
    Private(const QString &channelName, SIpcChannel *parent);
    SIpcConnection *mConnection;
    
    const QString &channelName() const;
    void sendMessage(const QString &message, const QByteArray &data);

    bool isServer() const;

public slots:
    void reconnect();

signals:
    void becameServer();
    
private slots:
    void onMessageArrived(const QString &message, const QByteArray &data);

private:
    SIpcChannel *q;
};

#endif // SIPCCHANNEL_P_H
