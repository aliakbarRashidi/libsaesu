/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, and all associated material(s), including but not limited
 * to documentation are protected by copyright. All rights reserved.
 * Copying, including reproducing, storing, adapting, translating, or
 * reverse-engineering any or all of this material requires prior written
 * consent. This material also contains confidential information which
 * may not be disclosed in any form without prior written consent.
 */

#ifndef SIPCCHANNEL_P_H
#define SIPCCHANNEL_P_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>

// Us
#include "sglobal.h"
#include "sipcchannel.h"
class SIpcConnection;

class SIpcChannel::Private : public QObject
{
    Q_OBJECT

public:
    Private(const QString &channelName, SIpcChannel *parent);
    SIpcConnection *mConnection;
    
    const QString &channelName() const;
    void sendMessage(const QString &message, const QByteArray &data);

    bool isServer() const;

public slots:
    void reconnect();

signals:
    void becameServer();
    
private slots:
    void onMessageArrived(const QString &message, const QByteArray &data);

private:
    SIpcChannel *q;
};

#endif // SIPCCHANNEL_P_H
