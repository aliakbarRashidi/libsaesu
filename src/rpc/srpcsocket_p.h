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

#ifndef SRPCSOCKET_P_H
#define SRPCSOCKET_P_H

// Qt
#include <QTcpSocket>
#include <QString>
#include <QTcpServer>

// Us
#include "sglobal.h"

class SRpcSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit SRpcSocket(QObject *parent = 0);
    virtual ~SRpcSocket();

    QByteArray buildCommand(quint8 commandToken, const QByteArray &data);
    void sendCommand(const QByteArray &data);
    void sendCommand(quint8 commandToken, const QByteArray &data);

signals:
    void messageRead(const QByteArray &message);

private slots:
    void onReadyRead();

private:
    quint32 mBytesExpected;
};

#endif // SRPCSOCKET_H

