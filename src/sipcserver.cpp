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

