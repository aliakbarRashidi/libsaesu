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

#ifndef SIPCSERVER_H
#define SIPCSERVER_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

// Us
#include "sglobal.h"
#include "sipcserver_p.h"
class SIpcConnection;

class SIpcServer : public QLocalServer
{
    Q_OBJECT
public:
    SIpcServer(QObject *parent = 0);
    void incomingConnection(quintptr socketDescriptor);

private slots:
    void onClientDisconnected();
    void onClientMessage(const QString &message, const QByteArray &data);
    
private:
    QList<SIpcConnection *> mPeers;
    QMultiMap<QString, SIpcConnection *> mChannels;
};

#endif // SIPCSERVER_H
