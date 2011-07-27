/*
 * Copyright (C) 2010-2011 Robin Burchell
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
