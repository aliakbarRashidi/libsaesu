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

#include <QHostInfo>
#include <QTcpSocket>
#include <QCryptographicHash>
#include <QtEndian>
#include <QMetaMethod>

#include "srpcservice.h"
#include "srpcservice_p.h"

#include "bonjourserviceregister.h"

/* base32_encode is:
 *
 * Copyright (c) 2001-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson
 * Copyright (c) 2007-2010, The Tor Project, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 *
 *   Neither the names of the copyright owners nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define BASE32_CHARS "abcdefghijklmnopqrstuvwxyz234567"

/* Implements base32 encoding as in rfc3548. Requires that srclen*8 is a multiple of 5. */
static void base32_encode(char *dest, unsigned destlen, const char *src, unsigned srclen)
{
    Q_UNUSED(destlen); // avoid warning

    unsigned i, bit, v, u;
    unsigned nbits = srclen * 8;

    Q_ASSERT((nbits%5) == 0); /* We need an even multiple of 5 bits. */
    Q_ASSERT((nbits/5)+1 <= destlen); /* We need enough space. */

    for (i = 0, bit = 0; bit < nbits; ++i, bit += 5)
    {
        /* set v to the 16-bit value starting at src[bits/8], 0-padded. */
        v = ((quint8) src[bit / 8]) << 8;
        if (bit + 5 < nbits)
            v += (quint8) src[(bit/8)+1];

        /* set u to the 5-bit value at the bit'th bit of src. */
        u = (v >> (11 - (bit % 8))) & 0x1F;
        dest[i] = BASE32_CHARS[u];
    }

    dest[i] = '\0';
}

SRpcServicePrivate::SRpcServicePrivate(QObject *parent, const QString &interfaceName)
     : QObject(parent)
     , mInterfaceName(interfaceName)
{
    connect(&mServer, SIGNAL(newConnection(SRpcSocket*)), SLOT(onNewConnection(SRpcSocket*)));

    int portNo;

    forever {
        portNo = qrand() % ((65535 + 1) - 49152) + 49152; // ephemeral port range

        if (!mServer.listen(QHostAddress::Any, portNo)) {
            if (mServer.serverError() == QAbstractSocket::AddressInUseError)
                continue;

            sWarning() << "Couldn't bind TCP; error: " << mServer.errorString();
            exit(1);
        }

        sDebug() << "Bound to port " << portNo;
        break;
    }

    QString srvRecordName = generateSrvName(interfaceName);
    QString serviceDesc = QString("%1 on %2").arg(interfaceName).arg(QHostInfo::localHostName());
    QString srvRecord = QLatin1String("_") + srvRecordName + QLatin1String("._tcp");

    BonjourServiceRegister *bonjourRegister = new BonjourServiceRegister(this);
    bonjourRegister->registerService(BonjourRecord(serviceDesc,
                                     srvRecord,
                                     QString()), portNo);
    sDebug() << "SRV record:" << srvRecordName << " created for interface " << interfaceName;
}

SRpcServicePrivate::~SRpcServicePrivate()
{
}

/*! Generates a SRV record name (like saesu-gmeczook) out of an interface name
 * (like org.saesu.syncd).
 */
QString SRpcServicePrivate::generateSrvName(const QString &interfaceName)
{
    // Generate a SRV-friendly name for the given interface name.
    // We do this by hashing the interface name, extracting the first bytes from
    // it (effectively truncating the hash) and base32ing that.
    //
    // SRV constraints can be read about in RFC 6335, but a simple summary of
    // the rules we really have to worry about is that we need something A-Z0-9-
    // no more than 15 characters in length, hence the hash truncation.
    //
    // This is safe to do, thanks to the avalanche effect
    // (http://en.wikipedia.org/wiki/Avalanche_effect).
    //
    // Thanks to John Brooks.
    //
    // TODO: sha256, either when Qt gets support, or by copying it.
    // See also: https://qt.gitorious.org/qt/qt/merge_requests/2475
    QByteArray hash = QCryptographicHash::hash(interfaceName.toUtf8(), QCryptographicHash::Sha1);

    QByteArray re;
    re.resize(8);
    base32_encode(re.data(), re.capacity(), hash.constData(), 5);

    return "saesu-" + re;
}

void SRpcServicePrivate::onNewConnection(SRpcSocket *socket)
{
    sDebug() << "Got a new connection from " << socket->peerAddress();
    connect(socket, SIGNAL(disconnected()), SLOT(onDisconnected()));
    connect(socket, SIGNAL(messageRead(const QByteArray&)), SLOT(processData(QByteArray)));
}

void SRpcServicePrivate::processData(QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);
    quint8 commandToken;
    ds >> commandToken;

    if (commandToken == 'A') {
        QByteArray methodName;
        QVariantHash parameters;

        ds >> methodName;
        ds >> parameters;
        sDebug() << "Running method " << methodName << " with parameters " << parameters;

        // find the method
        // TODO: should we keep a lookup list?
        // TODO: check access of methods
        int methodIdx = parent()->metaObject()->indexOfMethod(methodName.constData());
        QMetaMethod method = parent()->metaObject()->method(methodIdx);

        sDebug() << "Invoking";
        method.invoke(parent(), Qt::AutoConnection,
                Q_ARG(QVariantHash, parameters));
        sDebug() << "Invoked";
    }
}

void SRpcServicePrivate::onDisconnected()
{
    sDebug() << "Client disconnected";
    sender()->deleteLater();
}
