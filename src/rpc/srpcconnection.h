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

#ifndef SRPCCONNECTION_H
#define SRPCCONNECTION_H

// Qt
#include <QObject>
#include <QString>

#include "sglobal.h"
#include "srpcpeer.h"

class SRpcConnectionPrivate;

class SRpcConnection : public QObject
{
public:
    explicit SRpcConnection(const QString &interfaceName, QObject *parent = 0);
    explicit SRpcConnection(const QString &interfaceName, SRpcPeer peer, QObject *parent = 0);
    virtual ~SRpcConnection();

    void send(const QString &command, const QVariantHash &parameters);

private:
    SRpcConnectionPrivate *d;
};

#endif // SRPCCONNECTION_H
