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

    bool isServer() const;

signals:
    void received(const QString &message, const QByteArray &data);
    void becameServer();

private:
    class Private;
    Private *d;
};

#endif // SIPCCHANNEL_H

