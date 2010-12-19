// Qt
#include <QtNetwork/QLocalSocket>

// Us
#include "qtipcchannel.h"
#include "qtipcchannel_p.h"

QtIpcChannel::Private::Private(const QString &channelName, QtIpcChannel *parent)
    : QObject(parent)
    , mChannelName(channelName)
{
    reconnectOrEstablishServer();

    connect(&mServerInstance, SIGNAL(newConnection()), SLOT(onClientConnected()));
    connect(&mSlaveSocket, SIGNAL(disconnected()), SLOT(reconnectOrEstablishServer()));
}

void QtIpcChannel::Private::reconnectOrEstablishServer()
{
    mSlaveSocket.disconnectFromServer();

    while (mSlaveSocket.state() != QLocalSocket::ConnectedState &&
            !mServerInstance.isListening()) {
        // try connect to the server
        mSlaveSocket.connectToServer(QLatin1String("qtipcserver"));
        
        // XXX: this is going to hurt if it blocks hard
        // what would be nice would be a QLocalServer::isRegistered(const QString &name)
        if (!mSlaveSocket.waitForConnected()) {
            // couldn't connect; start a server instance
            // remove stale file just in caase
            QLocalServer::removeServer(QLatin1String("qtipcserver"));
            mServerInstance.listen(QLatin1String("qtipcserver"));
            sDebug() << "Established myself as the server";
        } else {
            // introduce ourselves to the server
//            mSlaveSocket.write("H " + mChannelName);
            sDebug() << "Connected and said hello";
        }
    }
}

void QtIpcChannel::Private::onClientConnected()
{
    sDebug() << "Got a new connection";
    
    while (mServerInstance.hasPendingConnections()) {
        QLocalSocket *s = mServerInstance.nextPendingConnection();
        registerPeer(s);
    }
}

void QtIpcChannel::Private::onClientDisconnected()
{
    sDebug() << "Connection closed";
    QLocalSocket *s = qobject_cast<QLocalSocket *>(sender());
    if (S_VERIFY(s, "no QLocalSocket instance"))
        return;
        
    mPeers.removeAll(s);
    s->deleteLater();
}

void QtIpcChannel::Private::registerPeer(QLocalSocket *s)
{
    connect(s, SIGNAL(disconnected()), SLOT(onClientDisconnected()));
    mPeers.append(s);
    sDebug() << "Registered peer " << s << ", total peers: " << mPeers.count();
}

