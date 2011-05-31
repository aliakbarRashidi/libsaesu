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
    void send() const;
    void isServer() const;
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

