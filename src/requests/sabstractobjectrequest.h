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
