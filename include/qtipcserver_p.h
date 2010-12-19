#ifndef QTIPCSERVER_H
#define QTIPCSERVER_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

// Us
#include "sglobal.h"
#include "qtipcserver_p.h"
class QtIpcConnection;

class QtIpcServer : public QLocalServer
{
    Q_OBJECT
public:
    QtIpcServer(QObject *parent = 0);
    void incomingConnection(quintptr socketDescriptor);

private slots:
    void onClientDisconnected();
    void onClientMessage(const QString &message, const QByteArray &data);
    
private:
    QList<QtIpcConnection *> mPeers;
    QMultiMap<QString, QtIpcConnection *> mChannels;
};

#endif // QTIPCSERVER_H
