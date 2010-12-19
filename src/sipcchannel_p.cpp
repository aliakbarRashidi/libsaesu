// Qt
#include <QtNetwork/QLocalSocket>

// Us
#include "sipcchannel.h"
#include "sipcchannel_p.h"
#include "sipcconnection_p.h"

SIpcChannel::Private::Private(const QString &channelName, SIpcChannel *parent)
    : QObject(parent)
    , mConnection(new SIpcConnection(this))
    , q(parent)
{
    connect(mConnection, SIGNAL(messageArrived(const QString &, const QByteArray &)),
                         SLOT(onMessageArrived(const QString &, const QByteArray &)));
    mConnection->setChannelName(channelName);
    mConnection->reconnect();
}

const QString &SIpcChannel::Private::channelName() const
{
    return mConnection->channelName();
}

void SIpcChannel::Private::sendMessage(const QString &message, const QByteArray &data)
{
    mConnection->sendMessage(message, data);
}

void SIpcChannel::Private::onMessageArrived(const QString &message, const QByteArray &data)
{
    q->receive(message, data);
}
