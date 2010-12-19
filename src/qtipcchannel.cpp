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
    sDebug() << "Got message on channel " << channel();
    sDebug() << "Message: " << message;
    sDebug() << "Data: " << data;
    emit received(message, data);
}

bool QtIpcChannel::isRegistered(const QString &channel)
{
    Q_UNUSED(channel);
    return false;
}

bool QtIpcChannel::send(const QString &channel, const QString &message)
{
    QtIpcChannel c(channel);
    c.send(message);

    return true;
}

bool QtIpcChannel::send(const QString &channel, const QString &message, const QByteArray& data)
{
    QtIpcChannel c(channel);
    c.send(message, data);

    return true;
}

bool QtIpcChannel::send(const QString &message)
{
    d->sendMessage(message, QByteArray());
    return true;
}

bool QtIpcChannel::send(const QString &message, const QByteArray &data)
{
    d->sendMessage(message, data);
    return true;
}

