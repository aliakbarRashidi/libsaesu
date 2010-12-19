/*
 * Copyright (C) 2010 Robin Burchell <robin.burchell@collabora.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
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

signals:
    void channelNameSet(const QString &channelName);
    void messageArrived(const QString &message, const QByteArray &data);
    
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
