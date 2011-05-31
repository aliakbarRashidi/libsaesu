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

#ifndef SIPCCONNECTION_H
#define SIPCCONNECTION_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

// Us
#include "sglobal.h"
#include "sipcchannel.h"
class SIpcServer;

class SIpcConnection : public QLocalSocket
{
    Q_OBJECT

public:
    SIpcConnection(QObject *parent = 0);
    SIpcServer *mServerInstance;
    
    void setChannelName(const QString &channelName);
    const QString &channelName() const;
    
    void sendMessage(const QString &message, const QByteArray &data);

    bool isServer() const;

signals:
    void channelNameSet(const QString &channelName);
    void messageArrived(const QString &message, const QByteArray &data);
    void becameServer();
    
public slots:
    void reconnect();
    
private slots:
    void onReadyRead();
    void processData(const QByteArray &bytes);
    
private:    
    quint32 mBytesExpected;
    QByteArray mBuffer;
    QString mChannelName;
};

#endif // SIPCCONNECTION_H
