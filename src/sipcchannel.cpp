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

#include <QLocalSocket>

// Us
#include "sipcchannel.h"
#include "sipcchannel_p.h"

SIpcChannel::SIpcChannel(const QString &channelName, QObject *parent)
    : QObject(parent)
    , d(new Private(channelName, this))
{
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

