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

// Qt
#include <QObject>

// Us
#include "srpcconnection.h"
#include "srpcconnection_p.h"

SRpcConnection::SRpcConnection(const QString &interfaceName, QObject *parent)
     : QObject(parent)
     , d(new SRpcConnectionPrivate(interfaceName, this))
{
}

SRpcConnection::SRpcConnection(const QString &interfaceName, SRpcPeer peer, QObject *parent)
    : QObject(parent)
    , d(new SRpcConnectionPrivate(interfaceName, this))
{
}

SRpcConnection::~SRpcConnection()
{
    sDebug() << "Destroying RPC connection";
}

void SRpcConnection::send(const QString &command, const QVariantHash &parameters)
{
    d->send(command, parameters);
}

