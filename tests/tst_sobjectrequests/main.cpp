/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
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
#include "sobjectmanager.h"
#include "sobjectfetchrequest.h"
#include "sobjectlocalidfilter.h"
#include "sobjectdetailfilter.h"
#include "sobjectsaverequest.h"
#include "sobjectremoverequest.h"
#include "sobjectid.h"

void tst_SObjectRequests::save()
{
    SObjectSaveRequest saveRequest;

    {
        SObject obj;
        obj.setValue("name", "Robin Burchell");
        obj.setValue("age", 23);

        saveRequest.add(obj);
    }
    
    {
        SObject obj;
        obj.setValue("name", "John Brooks");
        obj.setValue("age", 6);

        saveRequest.add(obj);
    }

    QSignalSpy requestStartSpy(&saveRequest, SIGNAL(started()));
    QSignalSpy requestFinishSpy(&saveRequest, SIGNAL(finished()));
    QSignalSpy objectsAddedSpy(&mManager, SIGNAL(objectsAdded(const QList<SObjectLocalId>&)));

    saveRequest.start(&mManager);
    QTest::qWait(1000);

    QCOMPARE(requestStartSpy.count(), 1);
    QCOMPARE(requestFinishSpy.count(), 1);
    QCOMPARE(objectsAddedSpy.count(), 1);

    // make sure they actually saved
    QList<SObject> objects = saveRequest.objects();
    QCOMPARE(objects.count(), 2);

    foreach (const SObject &object, objects) {
        QVERIFY(!object.id().localId().isNull());
    }
}

void tst_SObjectRequests::update()
{
    SObjectFetchRequest fetchRequest;
    SObjectDetailFilter filter;
    filter.setDetailName("name");
    filter.setValue("Robin Burchell");
    fetchRequest.setFilter(filter);

    QSignalSpy requestStartSpy(&fetchRequest, SIGNAL(started()));
    QSignalSpy requestFinishSpy(&fetchRequest, SIGNAL(finished()));

    fetchRequest.start(&mManager);
    QTest::qWait(1000);

    QCOMPARE(requestStartSpy.count(), 1);
    QCOMPARE(requestFinishSpy.count(), 1);

    QList<SObject> objects = fetchRequest.objects();

    QCOMPARE(objects.count(), 1);

    SObject object = objects.takeFirst();
    QCOMPARE(object.value("name"), QVariant(QString("Robin Burchell")));
    QCOMPARE(object.value("age"), QVariant(23));
}

// TODO: test update

void tst_SObjectRequests::remove()
{
    QList<SObject> objects;
    
    {
        SObjectFetchRequest fetchRequest;

        QSignalSpy requestStartSpy(&fetchRequest, SIGNAL(started()));
        QSignalSpy requestFinishSpy(&fetchRequest, SIGNAL(finished()));

        // no filter = fetch all
        fetchRequest.start(&mManager);
        QTest::qWait(1000);

        QCOMPARE(requestStartSpy.count(), 1);
        QCOMPARE(requestFinishSpy.count(), 1);

        objects = fetchRequest.objects();
        QCOMPARE(objects.count(), 2);
    }

    {
        SObjectRemoveRequest removeRequest;

        foreach (const SObject &object, objects) {
            removeRequest.add(object.id().localId());
        }

        QSignalSpy requestStartSpy(&removeRequest, SIGNAL(started()));
        QSignalSpy requestFinishSpy(&removeRequest, SIGNAL(finished()));
        QSignalSpy objectsRemovedSpy(&mManager, SIGNAL(objectsRemoved(const QList<SObjectLocalId>&)));

        removeRequest.start(&mManager);
        QTest::qWait(1000);

        QCOMPARE(requestStartSpy.count(), 1);
        QCOMPARE(requestFinishSpy.count(), 1);
        QCOMPARE(objectsRemovedSpy.count(), 1);

        // TODO: verify ids?
    }

    // verify removal
    {
        SObjectFetchRequest fetchRequest;

        QSignalSpy requestStartSpy(&fetchRequest, SIGNAL(started()));
        QSignalSpy requestFinishSpy(&fetchRequest, SIGNAL(finished()));

        // no filter = fetch all
        fetchRequest.start(&mManager);
        QTest::qWait(1000);

        QCOMPARE(requestStartSpy.count(), 1);
        QCOMPARE(requestFinishSpy.count(), 1);

        objects = fetchRequest.objects();
        QCOMPARE(objects.count(), 0);
    }
    // TODO: test leaving one object in the table
}

QTEST_MAIN(tst_SObjectRequests)
