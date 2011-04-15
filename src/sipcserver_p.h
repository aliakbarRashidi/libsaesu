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
