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

#ifndef SABSTRACTOBJECTREQUEST_H
#define SABSTRACTOBJECTREQUEST_H

#include <QObject>

#include "sglobal.h"
class SObjectManager;

class SAbstractObjectRequest : public QObject
{
    Q_OBJECT
public:
    explicit SAbstractObjectRequest(QObject *parent = 0);

    /*! Performs the request.
     */
    void start(SObjectManager *manager);

signals:
    void started();
    void finished();

protected:
    friend class SObjectManager;
    friend class SObjectSaveRequest;
    friend class SObjectFetchRequest;
    friend class SObjectRemoveRequest;
    friend class SDeleteListFetchRequest;

    class Private;
    Private *d;
};

#endif // SABSTRACTOBJECTREQUEST_H
