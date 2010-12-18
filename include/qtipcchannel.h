#ifndef QTIPCCHANNEL_H
#define QTIPCCHANNEL_H

// Qt
#include <QtCore/QObject>

// Us
#include "sglobal.h"

class QtIpcChannel : public QObject
{
    Q_OBJECT
public:
    QtIpcChannel(const QString &channelName, QObject *parent = 0);
    virtual	~QtIpcChannel() {}
    
    const QString &channel() const;
    virtual void receive(const QString &message, const QByteArray &data);
    
    static bool isRegistered(const QString &channel);
    static bool send(const QString &channel, const QString &message, const QByteArray& data);
    static bool send(const QString &channel, const QString &message);

signals:
    void received(const QString &message, const QByteArray &data);

private:
    class Private;
    Private *d;
};

#endif // QTIPCCHANNEL_H

