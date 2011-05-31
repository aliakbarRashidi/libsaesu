/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, and all associated material(s), including but not limited
 * to documentation are protected by copyright. All rights reserved.
 * Copying, including reproducing, storing, adapting, translating, or
 * reverse-engineering any or all of this material requires prior written
 * consent. This material also contains confidential information which
 * may not be disclosed in any form without prior written consent.
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
