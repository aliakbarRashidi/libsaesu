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

