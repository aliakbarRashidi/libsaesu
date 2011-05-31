/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, and all associated material(s), including but not limited
 * to documentation are protected by copyright. All rights reserved.
 * Copying, including reproducing, storing, adapting, translating, or
 * reverse-engineering any or all of this material requires prior written
 * consent. This material also contains confidential information which
 * may not be disclosed in any form without prior written consent.
 */

// Qt
#include <QtNetwork/QLocalSocket>

// Us
#include "sipcserver_p.h"
#include "sipcconnection_p.h"

#undef SIPCCONNECTION_DEBUG

SIpcConnection::SIpcConnection(QObject *parent)
    : QLocalSocket(parent)
    , mServerInstance(0)
    , mBytesExpected(0)
{
    connect(this, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

void SIpcConnection::reconnect()
{
    // if we were asked to connect once, we should always reconnect
#ifdef SIPCCONNECTION_DEBUG
    sDebug() << "Attempting reconnection";
#endif
    connect(this, SIGNAL(disconnected()), SLOT(reconnect()), Qt::UniqueConnection);
    disconnectFromServer();

    while (state() != QLocalSocket::ConnectedState) {
        // try connect to the server
        connectToServer(QLatin1String("sipcserver"));
        
        // XXX: this is going to hurt if it blocks hard
        // what would be nice would be a QLocalServer::isRegistered(const QString &name)
        if (!waitForConnected()) {
            if (isServer() && mServerInstance->isListening())
                qCritical("reconnect(): WTF: Couldn't connect when I was running the fucking server?");
                
            mServerInstance = new SIpcServer(this);
            emit becameServer();
        } else {
            // connected, introduce ourselves
            setChannelName(mChannelName);
        }
    }
}

void SIpcConnection::onReadyRead()
{
    while (bytesAvailable() >= sizeof(quint32)) {
        if (mBytesExpected == 0) {
            // read header
            read(reinterpret_cast<char *>(&mBytesExpected), sizeof(mBytesExpected));
        }

        if (bytesAvailable() < mBytesExpected)
            return;

        // read mBytesExpected and emit message arrived
        QByteArray bytes = read(mBytesExpected);
        Q_ASSERT((quint32)bytes.length() == mBytesExpected);
        
        processData(bytes);
        mBytesExpected = 0;
    }
}

void SIpcConnection::processData(const QByteArray &bytes)
{
    QDataStream stream(bytes);
    
    quint8 command;
    stream >> command;
    
    switch (command) {
        case 0x1:
            // setting channel
            stream >> mChannelName;
#ifdef SIPCCONNECTION_DEBUG
            sDebug() << "Set channel name to " << mChannelName << " for " << this;
#endif
            emit channelNameSet(mChannelName);
            break;
        case 0x2: {
            // sending a message, find message
            QString message;
            QByteArray data;
            
            stream >> message;
            stream >> data;
                    
            emit messageArrived(message, data);
            break;
        }
        default:
            break;
    }
}

void SIpcConnection::setChannelName(const QString &channelName)
{
    mChannelName = channelName;
    
    if (state() != ConnectedState)
        return;

    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    
    // we fix the length up later
    // length, command, channel name
    quint32 length = 0;
    stream << length;
    stream << (quint8)0x1;
    
    // channel name
    stream << channelName;
    
    // correct message length
    length = bytes.length() - sizeof(quint32);
    bytes[0] = length >> 0;
    bytes[1] = length >> 8;
    bytes[2] = length >> 16;
    bytes[3] = length >> 24;

    write(bytes);
#ifdef SIPCCONNECTION_DEBUG
    sDebug() << "Sent channel change request to " << channelName;
#endif
}

const QString &SIpcConnection::channelName() const
{
    return mChannelName;
}

void SIpcConnection::sendMessage(const QString &message, const QByteArray &data)
{
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    
    // we fix the length up later
    // length, command, message, data
    quint32 length = 0;
    stream << length;
    stream << (quint8)0x2;
    
    // message
    stream << message;
    
    // data
    stream << data;
    
    length = bytes.length() - sizeof(quint32);
    bytes[0] = length >> 0;
    bytes[1] = length >> 8;
    bytes[2] = length >> 16;
    bytes[3] = length >> 24;

    write(bytes);
    flush();
#ifdef SIPCCONNECTION_DEBUG
    sDebug() << "Sent message to " << mChannelName << ": " << message;
#endif
}

bool SIpcConnection::isServer() const
{
    return mServerInstance != 0;
}
