#ifndef QTIPCCHANNEL_P_H
#define QTIPCCHANNEL_P_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>

// Us
#include "sglobal.h"
#include "qtipcchannel.h"
class QtIpcConnection;

class QtIpcChannel::Private : public QObject
{
    Q_OBJECT

public:
    Private(const QString &channelName, QtIpcChannel *parent);
    QtIpcConnection *mConnection;
    
    const QString &channelName() const;
    void sendMessage(const QString &message, const QByteArray &data);
};

#endif // QTIPCCHANNEL_P_H
