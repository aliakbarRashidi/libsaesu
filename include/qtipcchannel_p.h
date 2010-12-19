#ifndef QTIPCCHANNEL_P_H
#define QTIPCCHANNEL_P_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

// Us
#include "sglobal.h"
#include "qtipcchannel.h"

class QtIpcChannel::Private : public QObject
{
    Q_OBJECT

public:
    Private(const QString &channelName, QtIpcChannel *parent);
    
    QString mChannelName;
    QLocalServer mServerInstance;
    QList<QLocalSocket *> mPeers;
    QLocalSocket mSlaveSocket;
    
private slots:
    void onClientConnected();
    void onClientDisconnected();
    void reconnectOrEstablishServer();
    
private:
    void registerPeer(QLocalSocket *s);
};

#endif // QTIPCCHANNEL_P_H
