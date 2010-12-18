#ifndef QTIPCCHANNEL_P_H
#define QTIPCCHANNEL_P_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>

// Us
#include "qtipcchannel.h"

class QtIpcChannel::Private : public QObject
{
    Q_OBJECT

public:
    Private(const QString &channelName, QtIpcChannel *parent);
    
    QString mChannelName;
};

#endif // QTIPCCHANNEL_P_H
