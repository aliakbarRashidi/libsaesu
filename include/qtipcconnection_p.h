#ifndef QTIPCCONNECTION_H
#define QTIPCCONNECTION_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

// Us
#include "sglobal.h"
#include "qtipcchannel.h"
class QtIpcServer;

class QtIpcConnection : public QLocalSocket
{
    Q_OBJECT

public:
    QtIpcConnection(QObject *parent = 0);
    QtIpcServer *mServerInstance;
    
    void setChannelName(const QString &channelName);
    const QString &channelName() const;
    
    void sendMessage(const QString &message, const QByteArray &data);

signals:
    void channelNameSet(const QString &channelName);
    void messageArrived(const QString &message, const QByteArray &data);
    
public slots:
    void reconnect();
    
private slots:
    void onReadyRead();
    void processData(const QByteArray &bytes);
    
private:    
    quint32 mBytesExpected;
    QByteArray mBuffer;
    QString mChannelName;
};

#endif // QTIPCCONNECTION_H
