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

// Qt
#include <QObject>

// Us
#include "sipcchannel.h"

class tst_SIpcChannel : public QObject
{
    Q_OBJECT
private slots:
    void channel() const;
    void receive() const;
};

class TestRecieverSink : public SIpcChannel
{
public:
    TestRecieverSink(const QString &channelName)
        : SIpcChannel(channelName)
        , mCalls(0)
    {}
    
    int mCalls;
    
    virtual void receive(const QString &message, const QByteArray &data)
    {
        mCalls++;
    }
};

