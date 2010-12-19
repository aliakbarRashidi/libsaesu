#ifndef SIPCCHANNEL_H
#define SIPCCHANNEL_H

// Qt
#include <QtCore/QObject>

// Us
#include "sglobal.h"

class SIpcChannel : public QObject
{
    Q_OBJECT
public:
    SIpcChannel(const QString &channelName, QObject *parent = 0);
    virtual	~SIpcChannel() {}
    
    const QString &channel() const;
    virtual void receive(const QString &message, const QByteArray &data);

    static bool send(const QString &channel, const QString &message, const QByteArray &data);
    static bool send(const QString &channel, const QString &message);
    
    bool sendMessage(const QString &message, const QByteArray &data);
    bool sendMessage(const QString &message);

signals:
    void received(const QString &message, const QByteArray &data);

private:
    class Private;
    Private *d;
};

#endif // SIPCCHANNEL_H

