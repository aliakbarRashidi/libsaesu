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
#include "sipcserver_p.h"
#include "sipcconnection_p.h"

#undef SIPCSERVER_DEBUG

SIpcServer::SIpcServer(QObject *parent)
    : QLocalServer(parent)
{
    // remove stale file just in caase
    QLocalServer::removeServer(QLatin1String("sipcserver"));
    this->listen(QLatin1String("sipcserver"));
#ifdef SIPCSERVER_DEBUG
    sDebug() << "Established myself as the server";
#endif
}

void SIpcServer::incomingConnection(quintptr socketDescriptor)
{
    SIpcConnection *s = new SIpcConnection(this);
    s->setSocketDescriptor(socketDescriptor);
    connect(s, SIGNAL(messageArrived(const QString &, const QByteArray &)),
               SLOT(onClientMessage(const QString &, const QByteArray &)));
    connect(s, SIGNAL(disconnected()), SLOT(onClientDisconnected()));
    mPeers.append(s);
#ifdef SIPCSERVER_DEBUG
    sDebug() << "Registered connection " << s << ", total peers: " << mPeers.count();
#endif
}

void SIpcServer::onClientDisconnected()
{
#ifdef SIPCSERVER_DEBUG
    sDebug() << "Connection closed";
#endif
    SIpcConnection *s = qobject_cast<SIpcConnection *>(sender());
    if (S_VERIFY(s, "no QLocalSocket instance"))
        return;
        
    mPeers.removeAll(s);
    s->deleteLater();
}

void SIpcServer::onClientMessage(const QString &message, const QByteArray &data)
{
    SIpcConnection *s = qobject_cast<SIpcConnection *>(sender());
    if (S_VERIFY(s, "no QLocalSocket instance"))
        return;

    foreach (SIpcConnection *conn, mPeers) {
        // ignore sender
        if (conn == s)
            continue;
    
        // if they're on the same channel, resend the message
        if (conn->channelName() == s->channelName())
            conn->sendMessage(message, data);
    } 
}

