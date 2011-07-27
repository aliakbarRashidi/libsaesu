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

#include <QtNetwork/QLocalSocket>
#include <QtCore/QTimer>

// Us
#include "sipcchannel.h"
#include "sipcchannel_p.h"

SIpcChannel::SIpcChannel(const QString &channelName, QObject *parent)
    : QObject(parent)
    , d(new Private(channelName, this))
{
    connect(d, SIGNAL(becameServer()), SIGNAL(becameServer()));
    QTimer::singleShot(0, d, SLOT(reconnect())); // slot, because we want creator of SIpcChannel to be able to connect signals.
}

const QString &SIpcChannel::channel() const
{
    return d->channelName();
}

void SIpcChannel::receive(const QString &message, const QByteArray &data)
{
    emit received(message, data);
}

bool SIpcChannel::send(const QString &channel, const QString &message)
{
    SIpcChannel c(channel);
    c.sendMessage(message);

    return true;
}

bool SIpcChannel::send(const QString &channel, const QString &message, const QByteArray& data)
{
    SIpcChannel c(channel);
    c.sendMessage(message, data);

    return true;
}

bool SIpcChannel::sendMessage(const QString &message)
{
    d->sendMessage(message, QByteArray());
    return true;
}

bool SIpcChannel::sendMessage(const QString &message, const QByteArray &data)
{
    d->sendMessage(message, data);
    return true;
}

/*!
 * Determines whether or not this SIpcChannel instance is the server.
 * A server instance is one which other channel instances connect to.
 *
 * If the server instance is destroyed, then another instance of SIpcChannel
 * will take over server duties locally, emitting becameServer() at the same time.
 *
 * \sa becameServer()
 */
bool SIpcChannel::isServer() const
{
    return d->isServer();
}
