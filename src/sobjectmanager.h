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

#ifndef SOBJECTMANAGER_H
#define SOBJECTMANAGER_H

// Qt
#include <QObject>
#include <QString>

// Us
class SAbstractObjectRequest;
#include "sobjectid.h"

class SObjectManager : public QObject
{
    Q_OBJECT
public:
    explicit SObjectManager(const QString &tableName, QObject *parent = 0);
    virtual ~SObjectManager();

signals:
    void objectsAdded(const QList<SObjectLocalId> &objectIds);
    void objectsUpdated(const QList<SObjectLocalId> &objectIds);
    void objectsRemoved(const QList<SObjectLocalId> &objectIds);

private:
    friend class SObjectSaveRequest;
    friend class SObjectRemoveRequest;
    friend class SObjectFetchRequest;
    friend class SDeleteListFetchRequest;

    class Private;
    Private *d;
};

#endif // SOBJECTMANAGER_H
