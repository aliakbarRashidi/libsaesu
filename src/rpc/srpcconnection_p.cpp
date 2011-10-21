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

    connect(&mSocket, SIGNAL(connected()), SLOT(onConnected()));
    connect(&mSocket, SIGNAL(disconnected()), SLOT(onDisconnected()));
    connect(&mSocket, SIGNAL(messageRead(const QByteArray&)), SLOT(processData(QByteArray)));
}

SRpcConnectionPrivate::~SRpcConnectionPrivate()
{
}

void SRpcConnectionPrivate::processData(QByteArray data)
{
    SRpcSocket *socket = qobject_cast<SRpcSocket *>(sender());
    if (S_VERIFY(socket, "no socket ptr"))
        return;

    QDataStream ds(&data, QIODevice::ReadOnly);
    quint8 commandToken;
    ds >> commandToken;

    if (commandToken == 'S') {
        QString signalName;
        QVariantHash parameters;

        ds >> signalName;
        ds >> parameters;
        sDebug() << "Got signal " << signalName << " params: " << parameters << " from " << mInterfaceName;
    }
}


void SRpcConnectionPrivate::onConnected()
{
    // TODO: move this to a connected callback
    if (!mPendingMessages.isEmpty()) {
        sDebug() << "Connected to RPC service for " << mInterfaceName << "; flushing message queue";
        foreach (const QByteArray &message, mPendingMessages)
            mSocket.sendCommand(message);

        mPendingMessages.clear();
    }
}

void SRpcConnectionPrivate::onDisconnected()
{
    // TODO: gracefully handle disconnection by attempting periodic reconnection
    sDebug() << "DISCONNECTED! ACK!";
    qCritical("SRpcConnectionPrivate::onDisconnected(): I can't handle disconnections");
}

void SRpcConnectionPrivate::updateRecords(const QList<BonjourRecord> &list)
{
    QStringList peerNames;

    foreach (const BonjourRecord &record, list) {
        mResolver.resolveBonjourRecord(record);
        peerNames.append(record.serviceName);
    }

    sDebug() << "Got peers: " << peerNames;
}

void SRpcConnectionPrivate::connectToServer(const QHostInfo &hostInfo, int port)
{
    if (mSocket.state() == QTcpSocket:: ConnectedState)
        return;

    // TODO: handle this properly by queueing addresses and trying each of them until we find one that works
    // TODO: this breaks ipv6 until we do
    foreach (const QHostAddress &remoteAddr, hostInfo.addresses()) {
        if (remoteAddr.protocol() == QAbstractSocket::IPv4Protocol) {
            sDebug() << "Got told to connect to " << remoteAddr << " on " << port << " for " << mInterfaceName;
            mSocket.connectToHost(remoteAddr, port);
            break;
        }
    }
}

void SRpcConnectionPrivate::invoke(const QString &command, const QVariantHash &parameters)
{
    sDebug() << "Sending RPC command" << command << "(" << parameters << ")";

    QString signature = command + "(const QVariantHash &)";

    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);
    ds << QByteArray(QMetaObject::normalizedSignature(signature.toLatin1()));
    ds << parameters;

    QByteArray commandPacket = mSocket.buildCommand('A', ba);

    if (mSocket.state() != QTcpSocket:: ConnectedState) {
        mPendingMessages.append(commandPacket);
        return;
    }

    mSocket.sendCommand(commandPacket);
}

void SRpcConnectionPrivate::listen(const QString &signal, QObject *object, const char *slot)
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);
    ds << signal;

    QByteArray commandPacket = mSocket.buildCommand('s', ba);

    if (mSocket.state() != QTcpSocket:: ConnectedState) {
        mPendingMessages.append(commandPacket);
        return;
    }

    mSocket.sendCommand(commandPacket);
}

