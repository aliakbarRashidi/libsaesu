#ifndef SIPCCHANNEL_P_H
#define SIPCCHANNEL_P_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>

// Us
#include "sglobal.h"
#include "sipcchannel.h"
class SIpcConnection;

class SIpcChannel::Private : public QObject
{
    Q_OBJECT

public:
    Private(const QString &channelName, SIpcChannel *parent);
    SIpcConnection *mConnection;
    
    const QString &channelName() const;
    void sendMessage(const QString &message, const QByteArray &data);
    
private slots:
    void onMessageArrived(const QString &message, const QByteArray &data);
    
private:
    SIpcChannel *q;
};

#endif // SIPCCHANNEL_P_H
