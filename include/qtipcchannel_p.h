#ifndef QTIPCCHANNEL_P_H
#define QTIPCCHANNEL_P_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QUdpSocket>

// Us
#include "sglobal.h"
#include "qtipcchannel.h"

class QtIpcChannel::Private : public QObject
{
    Q_OBJECT

public:
    Private(const QString &channelName, QtIpcChannel *parent);
    
    QString mChannelName;
    QUdpSocket mBroadcaster;
    QLocalServer mServerInstance;
    QList<QLocalSocket *> mPeers;
    
private slots:
    void onNewIpcChannelAnnounced();
    void onPeerConnected();
    void onPeerDisconnected();
    void onPeerError(QLocalSocket::LocalSocketError socketError);
    
private:
    void registerPeer(QLocalSocket *s);
};

#endif // QTIPCCHANNEL_P_H
