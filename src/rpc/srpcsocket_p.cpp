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

#include <QTcpSocket>
#include <QtEndian>

#include "srpcsocket_p.h"

SRpcSocket::SRpcSocket(QObject *parent)
     : QTcpSocket(parent)
     , mBytesExpected(0)
{
    connect(this, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

SRpcSocket::~SRpcSocket()
{
}

void SRpcSocket::sendCommand(quint8 commandToken, const QByteArray &data)
{
    quint32 length = qToBigEndian<quint32>((quint32)data.length() + 1);
    write(reinterpret_cast<char *>(&length), sizeof(quint32));
    write(reinterpret_cast<char *>(&commandToken), sizeof(quint8));
    write(data);
}


void SRpcSocket::onReadyRead()
{
    while (bytesAvailable() >= sizeof(quint32)) {
        if (mBytesExpected == 0) {
            // read header
            read(reinterpret_cast<char *>(&mBytesExpected), sizeof(mBytesExpected));
            mBytesExpected = qFromBigEndian<quint32>(mBytesExpected);
        }

        if (bytesAvailable() < mBytesExpected) {
            sDebug() << "Got " << bytesAvailable() << "; want " << mBytesExpected;
            return;
        }

        // read bytesExpected bytes and process it
        QByteArray bytes = read(mBytesExpected);
        Q_ASSERT((quint32)bytes.length() == mBytesExpected);

        sDebug() << "Read a message of " << bytes.length() << " bytes";
        emit messageRead(bytes);
        mBytesExpected = 0;
    }

}

