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
#include <QtNetwork/QLocalSocket>

// Us
#include "sipcchannel.h"
#include "sipcchannel_p.h"
#include "sipcconnection_p.h"

SIpcChannel::Private::Private(const QString &channelName, SIpcChannel *parent)
    : QObject(parent)
    , mConnection(new SIpcConnection(this))
    , q(parent)
{
    connect(mConnection, SIGNAL(becameServer()), SIGNAL(becameServer()));
    connect(mConnection, SIGNAL(messageArrived(const QString &, const QByteArray &)),
                         SLOT(onMessageArrived(const QString &, const QByteArray &)));
    mConnection->setChannelName(channelName);
}

const QString &SIpcChannel::Private::channelName() const
{
    return mConnection->channelName();
}

void SIpcChannel::Private::sendMessage(const QString &message, const QByteArray &data)
{
    mConnection->sendMessage(message, data);
}

void SIpcChannel::Private::onMessageArrived(const QString &message, const QByteArray &data)
{
    q->receive(message, data);
}

bool SIpcChannel::Private::isServer() const
{
    return mConnection->isServer();
}

void SIpcChannel::Private::reconnect()
{
    mConnection->reconnect();
}
