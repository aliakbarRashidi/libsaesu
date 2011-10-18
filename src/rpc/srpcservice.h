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

#ifndef SRPCSERVICE_H
#define SRPCSERVICE_H

// Qt
#include <QObject>
#include <QString>

// saesu
#include "sglobal.h"
class SRpcServicePrivate;

class SRpcService : public QObject
{
public:
    explicit SRpcService(const QString &interfaceName, QObject *parent = 0);
    virtual ~SRpcService();

private:
    SRpcServicePrivate *d;
};

#endif // SRPCSERVICE_H