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
#include "sipclistener.h"
#include "sipclistener_p.h"

SIpcListener::SIpcListener(QObject *parent)
     : QObject(parent)
     , d(new Private(this))
{
}

SIpcListener::~SIpcListener()
{
}

void SIpcListener::listen(SRpcConnection *connection, const QString &interface, const QString &signal, const char *slot)
{

}

