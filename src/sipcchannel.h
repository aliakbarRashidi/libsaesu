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

