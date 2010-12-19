#include <QLocalSocket>

// Us
#include "sipcchannel.h"
#include "sipcchannel_p.h"

SIpcChannel::SIpcChannel(const QString &channelName, QObject *parent)
    : QObject(parent)
    , d(new Private(channelName, this))
{
}

const QString &SIpcChannel::channel() const
{
    return d->channelName();
}

void SIpcChannel::receive(const QString &message, const QByteArray &data)
{
    emit received(message, data);
}

bool SIpcChannel::send(const QString &channel, const QString &message)
{
    SIpcChannel c(channel);
    c.sendMessage(message);

    return true;
}

bool SIpcChannel::send(const QString &channel, const QString &message, const QByteArray& data)
{
    SIpcChannel c(channel);
    c.sendMessage(message, data);

    return true;
}

bool SIpcChannel::sendMessage(const QString &message)
{
    d->sendMessage(message, QByteArray());
    return true;
}

bool SIpcChannel::sendMessage(const QString &message, const QByteArray &data)
{
    d->sendMessage(message, data);
    return true;
}

