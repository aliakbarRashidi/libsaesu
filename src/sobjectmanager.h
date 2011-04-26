/*
 * Copyright (C) 2011 Robin Burchell <robin.burchell@collabora.co.uk>
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
