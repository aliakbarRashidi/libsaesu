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

#include <QStringList>
#include <QTcpSocket>
#include <QtEndian>

#include "srpcconnection.h"
#include "srpcconnection_p.h"
#include "srpcservice_p.h"

#include "bonjourservicebrowser.h"
#include "bonjourserviceresolver.h"

SRpcConnectionPrivate::SRpcConnectionPrivate(const QString &interfaceName, QObject *parent)
     : QObject(parent)
     , mInterfaceName(interfaceName)
{
    sDebug() << "Creating RPC connection to " << interfaceName;

    // TODO: we should only do this for connections where an SRpcPeer is not
    // provided; and we should limit ourselves to local providers only
    BonjourServiceBrowser *bonjourBrowser = new BonjourServiceBrowser(this);
    connect(bonjourBrowser, SIGNAL(currentBonjourRecordsChanged(const QList<BonjourRecord> &)),
            this, SLOT(updateRecords(const QList<BonjourRecord> &)));
    bonjourBrowser->browseForServiceType(QLatin1String("_") +
            SRpcServicePrivate::generateSrvName(interfaceName) + QLatin1String("._tcp"));

    connect(&mResolver, SIGNAL(bonjourRecordResolved(const QHostInfo &, int)),
            this, SLOT(connectToServer(const QHostInfo &, int)));
}

SRpcConnectionPrivate::~SRpcConnectionPrivate()
{
}

void SRpcConnectionPrivate::updateRecords(const QList<BonjourRecord> &list)
{
    QStringList peerNames;
    sDebug() << list.count() << " records";

    foreach (const BonjourRecord &record, list) {
        mResolver.resolveBonjourRecord(record);
        peerNames.append(record.serviceName);
    }

    sDebug() << "Got peers: " << peerNames;
}

static void sendCommand(QTcpSocket *socket, quint8 token, const QByteArray &data)
{
    quint32 length = qToBigEndian<quint32>((quint32)data.length() + 1);
    socket->write(reinterpret_cast<char *>(&length), sizeof(quint32));
    socket->write(reinterpret_cast<char *>(&token), sizeof(quint8));
    socket->write(data);
}

void SRpcConnectionPrivate::connectToServer(const QHostInfo &hostInfo, int port)
{
    // TODO: handle this properly by queueing addresses and trying each of them until we find one that works
    // TODO: this breaks ipv6 until we do
    foreach (const QHostAddress &remoteAddr, hostInfo.addresses()) {
        if (remoteAddr.protocol() == QAbstractSocket::IPv4Protocol) {
            // TODO: don't open multiple sockets
            sDebug() << "Got told to connect to " << remoteAddr << " on " << port << " for " << mInterfaceName;
            QTcpSocket *socket = new QTcpSocket(this);
            socket->connectToHost(remoteAddr, port);
            sendCommand(socket, 'A', QByteArray("this is a byte array"));
            break;
        }
    }
}
