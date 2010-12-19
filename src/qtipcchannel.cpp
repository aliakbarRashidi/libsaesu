#include <QLocalSocket>

// Us
#include "qtipcchannel.h"
#include "qtipcchannel_p.h"

QtIpcChannel::QtIpcChannel(const QString &channelName, QObject *parent)
    : QObject(parent)
    , d(new Private(channelName, this))
{
}

const QString &QtIpcChannel::channel() const
{
    return d->channelName();
}

void QtIpcChannel::receive(const QString &message, const QByteArray &data)
{
    emit received(message, data);
}

bool QtIpcChannel::send(const QString &channel, const QString &message)
{
    QtIpcChannel c(channel);
    c.sendMessage(message);

    return true;
}

bool QtIpcChannel::send(const QString &channel, const QString &message, const QByteArray& data)
{
    QtIpcChannel c(channel);
    c.sendMessage(message, data);

    return true;
}

bool QtIpcChannel::sendMessage(const QString &message)
{
    d->sendMessage(message, QByteArray());
    return true;
}

bool QtIpcChannel::sendMessage(const QString &message, const QByteArray &data)
{
    d->sendMessage(message, data);
    return true;
}

