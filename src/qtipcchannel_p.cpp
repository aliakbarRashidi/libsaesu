// Qt
#include <QtNetwork/QLocalSocket>

// Us
#include "qtipcchannel.h"
#include "qtipcchannel_p.h"

QtIpcChannel::Private::Private(const QString &channelName, QtIpcChannel *parent)
    : QObject(parent)
    , mChannelName(channelName)
{
    // find a unique free name
    int tries = 0;
    QString realName;
    forever {
        QLocalSocket socket;
        realName = channelName + QString::number(tries);
        sDebug() << "Trying to connect to " << realName;
        socket.connectToServer(realName);
        
        // XXX: this is going to hurt if it blocks hard
        // what would be nice would be a QLocalServer::isRegistered(const QString &name)
        if (!socket.waitForConnected())
            break; // couldn't connect, free for a server
            
        tries++;
    }
    
    sDebug() << "Listening to " << realName;
    
    // remove stale file just in caase
    QLocalServer::removeServer(realName);
    mServerInstance.listen(realName);
    connect(&mServerInstance, SIGNAL(newConnection()), SLOT(onPeerConnected()));
    
    // broadcast our presence
    QString packet = channelName + QLatin1Char('\n') + realName + QLatin1Char('\n');
    mBroadcaster.bind(QHostAddress::LocalHost, 12354);
    sDebug() << "Broadcasting: " << mBroadcaster.writeDatagram(packet.toUtf8(), QHostAddress::LocalHost, 12354);

    connect(&mBroadcaster, SIGNAL(readyRead()), SLOT(onNewIpcChannelAnnounced()));
}

void QtIpcChannel::Private::onNewIpcChannelAnnounced()
{
    static QByteArray buffer;
    while (mBroadcaster.hasPendingDatagrams()) {
         QByteArray datagram;
         buffer.resize(mBroadcaster.pendingDatagramSize());
         mBroadcaster.readDatagram(buffer.data(), buffer.size());
    }
    
    QList<QByteArray> bufferParts = buffer.split('\n');
    QString channelName;
    QString socketAddress;
    
    foreach (const QByteArray &bufferPart, bufferParts) {
        if (channelName.isEmpty()) {
            channelName = bufferPart;
        } else {
            socketAddress = bufferPart;

            sDebug() << "Got channel: " << channelName
                     << " with address " << socketAddress;
                     
            if (socketAddress != mServerInstance.serverName()) {
                sDebug() << "Connecting to " << socketAddress;
                QLocalSocket *s = new QLocalSocket(this);
                s->connectToServer(socketAddress);
                
                // XXX: blocking
                if (!s->waitForConnected()) {
                    sDebug() << "Couldn't connect! " << s->errorString();
                    delete s;
                    
                    // TODO: what to do here? this effectively means IPC is fucked
                } else {
                    registerPeer(s);
                }
            }
                     
            channelName.clear();
            socketAddress.clear();
        }
    }
}

void QtIpcChannel::Private::onPeerConnected()
{
    sDebug() << "Got a new connection";
    
    while (mServerInstance.hasPendingConnections()) {
        QLocalSocket *s = mServerInstance.nextPendingConnection();
        registerPeer(s);
    }
}

void QtIpcChannel::Private::onPeerDisconnected()
{
    sDebug() << "Connection closed";
    QLocalSocket *s = qobject_cast<QLocalSocket *>(sender());
    if (S_VERIFY(s, "no QLocalSocket instance"))
        return;
        
    mPeers.removeAll(s);
    s->deleteLater();
}

void QtIpcChannel::Private::onPeerError(QLocalSocket::LocalSocketError socketError)
{
    sDebug() << "Connection error: " << socketError;
    onPeerDisconnected();
}

void QtIpcChannel::Private::registerPeer(QLocalSocket *s)
{
    connect(s, SIGNAL(disconnected()), SLOT(onPeerDisconnected()));
    connect(s, SIGNAL(error(QLocalSocket::LocalSocketError)),
               SLOT(onPeerError(QLocalSocket::LocalSocketError)));
    mPeers.append(s);
    sDebug() << "Registered peer " << s << ", total peers: " << mPeers.count();
}

