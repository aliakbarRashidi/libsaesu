/*
 * Copyright (C) 2011 Robin Burchell
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

#ifndef SIPCLISTENER_H
#define SIPCLISTENER_H

// Qt
#include <QObject>
#include <QString>

// saesu
#include "srpcconnection.h"

class SIpcListener : public QObject
{
public:
    explicit SIpcListener(QObject *parent);
    virtual ~SIpcListener();

    void listen(SRpcConnection *connection, const QString &interface, const QString &signal, const char *slot);

private:
    class Private;
    Private *d;
};

#endif // SIPCLISTENER_H
