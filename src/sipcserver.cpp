// Qt
#include <QtNetwork/QLocalSocket>

// Us
#include "sipcserver_p.h"
#include "sipcconnection_p.h"

SIpcServer::SIpcServer(QObject *parent)
    : QLocalServer(parent)
{
    // remove stale file just in caase
    QLocalServer::removeServer(QLatin1String("sipcserver"));
    this->listen(QLatin1String("sipcserver"));
    sDebug() << "Established myself as the server";
}

void SIpcServer::incomingConnection(quintptr socketDescriptor)
{
    SIpcConnection *s = new SIpcConnection(this);
    s->setSocketDescriptor(socketDescriptor);
    connect(s, SIGNAL(messageArrived(const QString &, const QByteArray &)),
               SLOT(onClientMessage(const QString &, const QByteArray &)));
    connect(s, SIGNAL(disconnected()), SLOT(onClientDisconnected()));
    mPeers.append(s);
    sDebug() << "Registered connection " << s << ", total peers: " << mPeers.count();
}

void SIpcServer::onClientDisconnected()
{
    sDebug() << "Connection closed";
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

