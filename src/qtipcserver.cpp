// Qt
#include <QtNetwork/QLocalSocket>

// Us
#include "qtipcserver_p.h"
#include "qtipcconnection_p.h"

QtIpcServer::QtIpcServer(QObject *parent)
    : QLocalServer(parent)
{
    // remove stale file just in caase
    QLocalServer::removeServer(QLatin1String("qtipcserver"));
    this->listen(QLatin1String("qtipcserver"));
    sDebug() << "Established myself as the server";
}

void QtIpcServer::incomingConnection(quintptr socketDescriptor)
{
    QtIpcConnection *s = new QtIpcConnection(this);
    s->setSocketDescriptor(socketDescriptor);
    connect(s, SIGNAL(messageArrived(const QString &, const QByteArray &)),
               SLOT(onClientMessage(const QString &, const QByteArray &)));
    connect(s, SIGNAL(disconnected()), SLOT(onClientDisconnected()));
    mPeers.append(s);
    sDebug() << "Registered connection " << s << ", total peers: " << mPeers.count();
}

void QtIpcServer::onClientDisconnected()
{
    sDebug() << "Connection closed";
    QtIpcConnection *s = qobject_cast<QtIpcConnection *>(sender());
    if (S_VERIFY(s, "no QLocalSocket instance"))
        return;
        
    mPeers.removeAll(s);
    s->deleteLater();
}

void QtIpcServer::onClientMessage(const QString &message, const QByteArray &data)
{
    QtIpcConnection *s = qobject_cast<QtIpcConnection *>(sender());
    if (S_VERIFY(s, "no QLocalSocket instance"))
        return;

    foreach (QtIpcConnection *conn, mPeers) {
        // ignore sender
        if (conn == s)
            continue;
    
        // if they're on the same channel, resend the message
        if (conn->channelName() == s->channelName())
            conn->sendMessage(message, data);
    } 
}

