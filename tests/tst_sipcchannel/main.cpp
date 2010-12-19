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

QTEST_MAIN(tst_SIpcChannel)
