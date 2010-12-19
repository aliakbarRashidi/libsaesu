/*
 * Copyright (C) 2010 Robin Burchell <robin.burchell@collabora.co.uk>
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
    connect(mConnection, SIGNAL(messageArrived(const QString &, const QByteArray &)),
                         SLOT(onMessageArrived(const QString &, const QByteArray &)));
    mConnection->setChannelName(channelName);
    mConnection->reconnect();
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
