// Qt
#include <QtNetwork/QLocalSocket>

// Us
#include "qtipcserver_p.h"
#include "qtipcconnection_p.h"

QtIpcConnection::QtIpcConnection(QObject *parent)
    : QLocalSocket(parent)
    , mServerInstance(0)
    , mBytesExpected(0)
{
    connect(this, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

void QtIpcConnection::reconnect()
{
    // if we were asked to connect once, we should always reconnect
    connect(this, SIGNAL(disconnected()), SLOT(reconnect()), Qt::UniqueConnection);
    disconnectFromServer();

    while (state() != QLocalSocket::ConnectedState) {
        // try connect to the server
        connectToServer(QLatin1String("qtipcserver"));
        
        // XXX: this is going to hurt if it blocks hard
        // what would be nice would be a QLocalServer::isRegistered(const QString &name)
        if (!waitForConnected()) {
            if (mServerInstance && mServerInstance->isListening())
                qCritical("reconnect(): WTF: Couldn't connect when I was running the fucking server?");
                
            mServerInstance = new QtIpcServer(this);
        } else {
            // connected, introduce ourselves
            setChannelName(mChannelName);
        }
    }
}

void QtIpcConnection::onReadyRead()
{
    sDebug() << "bytes available: " << bytesAvailable() << peek(INT_MAX).toHex();

    while (bytesAvailable() >= sizeof(quint32)) {
        if (mBytesExpected == 0) {
            // read header
            read(reinterpret_cast<char *>(&mBytesExpected), sizeof(mBytesExpected));
        }

        sDebug() << "Expecting " << mBytesExpected << " bytes";       
        sDebug() << "Got " << bytesAvailable() << " bytes";
        
        if (bytesAvailable() < mBytesExpected)
            return;

        // read mBytesExpected and emit message arrived
        QByteArray bytes = read(mBytesExpected);
        Q_ASSERT(bytes.length() == mBytesExpected);
        
        processData(bytes);
        mBytesExpected = 0;
    }
}

void QtIpcConnection::processData(const QByteArray &bytes)
{
    sDebug() << "Processing " << bytes.length() << " bytes: " << bytes.toHex();
    QDataStream stream(bytes);
    
    quint8 command;
    stream >> command;
    
    switch (command) {
        case 0x1:
            // setting channel
            stream >> mChannelName;
            sDebug() << "Set channel name to " << mChannelName << " for " << this;
            emit channelNameSet(mChannelName);
            break;
        case 0x2: {
            // sending a message, find message
            QString message;
            QByteArray data;
            
            stream >> message;
            stream >> data;

            sDebug() << "Got message from channel: " << mChannelName;
            sDebug() << "Message: " << message;
            sDebug() << "Data: " << data;
                    
            emit messageArrived(message, data);
            break;
        }
        default:
            break;
    }
}

void QtIpcConnection::setChannelName(const QString &channelName)
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
    sDebug() << "Sent channel change request to " << channelName << ": " << bytes.toHex();
}

const QString &QtIpcConnection::channelName() const
{
    return mChannelName;
}

void QtIpcConnection::sendMessage(const QString &message, const QByteArray &data)
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
    sDebug() << "Sent message to " << mChannelName;
}

