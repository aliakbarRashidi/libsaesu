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

#include <QDataStream>
#include <QStringList>

#include "speermodel.h"
#include "speermodel_p.h"

SPeerModelPrivate::SPeerModelPrivate(QObject *parent)
     : QObject(parent)
{
}

SPeerModelPrivate::~SPeerModelPrivate()
{
}

void SPeerModelPrivate::onReceived(const QString &message, QByteArray data)
{
    if (message == "peersAvailable(QStringList)") {
        QDataStream ds(&data, QIODevice::ReadOnly);
        QStringList peers;
        ds >> peers;

        sDebug() << "Got peers: " << peers;
    }
}

