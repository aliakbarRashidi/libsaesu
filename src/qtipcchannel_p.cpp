// Qt
#include <QtNetwork/QLocalSocket>

// Us
#include "qtipcchannel.h"
#include "qtipcchannel_p.h"
#include "qtipcconnection_p.h"

QtIpcChannel::Private::Private(const QString &channelName, QtIpcChannel *parent)
    : QObject(parent)
    , mConnection(new QtIpcConnection(this))
{
    mConnection->setChannelName(channelName);
    mConnection->reconnect();
}

const QString &QtIpcChannel::Private::channelName() const
{
    return mConnection->channelName();
}

void QtIpcChannel::Private::sendMessage(const QString &message, const QByteArray &data)
{
    mConnection->sendMessage(message, data);
}

