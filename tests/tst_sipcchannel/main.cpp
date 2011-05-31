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
#include <QTest>
#include <QSignalSpy>

// Us
#include "main.h"

void tst_SIpcChannel::channel() const
{
    SIpcChannel s("test");
    QCOMPARE(s.channel(), QLatin1String("test"));

    SIpcChannel stwo("testtwo");
    QCOMPARE(stwo.channel(), QLatin1String("testtwo"));
}

void tst_SIpcChannel::receive() const
{
    {
        TestRecieverSink sink("test");
        TestRecieverSink sender("test");
        QSignalSpy spy(&sink, SIGNAL(received(QString, QByteArray)));
        
        sender.sendMessage("hello");
        sender.sendMessage("hello");
        sender.sendMessage("hello");
        
        QTest::qWait(500);
        
        // since we overrided receive(), received() should not have been emitted
        QCOMPARE(sink.mCalls, 3);
        QCOMPARE(spy.count(), 0);
    }
    
    {
        SIpcChannel sink("test");
        SIpcChannel sender("test");
        QSignalSpy spy(&sink, SIGNAL(received(QString, QByteArray)));
        
        sender.sendMessage("hello", "there");
        sender.sendMessage("hello");
        sender.sendMessage("hello");

        QTest::qWait(500);

        // make sure it emitted ok
        QCOMPARE(spy.count(), 3);

        // make sure we got the right thing
        QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(arguments.at(0).type(), QVariant::String);
        QCOMPARE(arguments.at(1).type(), QVariant::ByteArray);
        QCOMPARE(arguments.at(0).toString(), QLatin1String("hello"));
        QCOMPARE(arguments.at(1).toString(), QLatin1String("there"));
    }
}

void tst_SIpcChannel::send() const
{
    SIpcChannel sink("test");
    QSignalSpy spy(&sink, SIGNAL(received(QString, QByteArray)));

    SIpcChannel::send("test", "hello", "there");
    SIpcChannel::send("test", "hello");

    QTest::qWait(500);

    // make sure it emitted ok
    QCOMPARE(spy.count(), 2);

    // make sure we got the right thing
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).type(), QVariant::String);
    QCOMPARE(arguments.at(1).type(), QVariant::ByteArray);
    QCOMPARE(arguments.at(0).toString(), QLatin1String("hello"));
    QCOMPARE(arguments.at(1).toString(), QLatin1String("there"));
}

void tst_SIpcChannel::isServer() const
{
    {
        SIpcChannel test("test");
        QVERIFY(test.isServer());

        // creating another instance shouldn't kill it
        SIpcChannel testtwo("test");
        QVERIFY(test.isServer());
        QVERIFY(!testtwo.isServer());
    }

    {
        SIpcChannel *test = new SIpcChannel("test");
        QVERIFY(test->isServer());

        SIpcChannel *testtwo = new SIpcChannel("testtwo");
        QSignalSpy spy(testtwo, SIGNAL(becameServer()));
        QVERIFY(test->isServer());
        QVERIFY(!testtwo->isServer());

        // now delete test
        delete test;
        QTest::qWait(500);

        QVERIFY(testtwo->isServer());
        QCOMPARE(spy.count(), 1); // verify becameServer emitted ok
    }
}

QTEST_MAIN(tst_SIpcChannel)
