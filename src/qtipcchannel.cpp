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
    // default implementation does nothing
    Q_UNUSED(message);
    Q_UNUSED(data);
}

bool QtIpcChannel::isRegistered(const QString &channel)
{
    Q_UNUSED(channel);
    return false;
}

bool QtIpcChannel::send(const QString &channel, const QString &message)
{
    Q_UNUSED(channel);
    Q_UNUSED(message);

    return false;
}

bool QtIpcChannel::send(const QString &channel, const QString &message, const QByteArray& data)
{
    Q_UNUSED(channel);
    Q_UNUSED(message);
    Q_UNUSED(data);

    return false;
}

bool QtIpcChannel::send(const QString &message)
{
    d->sendMessage(message, QByteArray());
    return false;
}

bool QtIpcChannel::send(const QString &message, const QByteArray &data)
{
    d->sendMessage(message, data);
    return false;
}

