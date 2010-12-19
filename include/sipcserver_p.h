#ifndef SIPCSERVER_H
#define SIPCSERVER_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

// Us
#include "sglobal.h"
#include "sipcserver_p.h"
class SIpcConnection;

class SIpcServer : public QLocalServer
{
    Q_OBJECT
public:
    SIpcServer(QObject *parent = 0);
    void incomingConnection(quintptr socketDescriptor);

private slots:
    void onClientDisconnected();
    void onClientMessage(const QString &message, const QByteArray &data);
    
private:
    QList<SIpcConnection *> mPeers;
    QMultiMap<QString, SIpcConnection *> mChannels;
};

#endif // SIPCSERVER_H
