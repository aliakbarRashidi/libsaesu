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

#ifndef SPEERMODEL_P_H
#define SPEERMODEL_P_H

// Qt
#include <QObject>
#include <QString>
#include <QByteArray>

// Us
#include "sipcchannel.h"
#include "speermodel.h"

class SPeerModelPrivate : public QObject
{
    Q_OBJECT
public:
    explicit SPeerModelPrivate(QObject *parent);
    virtual ~SPeerModelPrivate();

private slots:
    void onReceived(const QString &message, QByteArray data);

private:

    SIpcChannel mPeerListener;
};

#endif // SPEERMODEL_H
