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

#ifndef SIPCCHANNEL_H
#define SIPCCHANNEL_H

// Qt
#include <QtCore/QObject>

// Us
#include "sglobal.h"

class SIpcChannel : public QObject
{
    Q_OBJECT
public:
    SIpcChannel(const QString &channelName, QObject *parent = 0);
    virtual	~SIpcChannel() {}
    
    const QString &channel() const;
    virtual void receive(const QString &message, const QByteArray &data);

    static bool send(const QString &channel, const QString &message, const QByteArray &data);
    static bool send(const QString &channel, const QString &message);
    
    bool sendMessage(const QString &message, const QByteArray &data);
    bool sendMessage(const QString &message);

    bool isServer() const;

signals:
    void received(const QString &message, const QByteArray &data);
    void becameServer();

private:
    class Private;
    Private *d;
};

#endif // SIPCCHANNEL_H

