/*
 * Copyright (C) 2010 Robin Burchell <robin.burchell@collabora.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

// Qt
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QCryptographicHash>

// Us
#include "sclouditem.h"
#include "sclouditem_p.h"

SCloudItem::Private::Private(const QString &cloudName, SCloudItem *parent)
    : q(parent)
    , mCloudName(cloudName)
    , mDeserialising(false)
{
    mUuid = QUuid::createUuid().toString();
    mUuid = mUuid.mid(1, mUuid.length() - 2);
}

SCloudItem::Private::~Private()
{
}

void SCloudItem::Private::objectModified()
{
    // ignore property changes while deserialising;
    // this would change lastModified (and therefore our hash) which is a bad thing
    if (mDeserialising)
        return;

    const QMetaObject *myMetaObject = q->metaObject();
    QCryptographicHash hasher(QCryptographicHash::Sha1);

    int i = 0;
    while (i < myMetaObject->propertyCount()) {
        QMetaProperty prop = myMetaObject->property(i);

        if (!prop.isReadable()) {
            sDebug() << "Skipping unreadable property" << prop.name();
            continue;
        }
        
        if (!prop.isWritable()) {
            sDebug() << "Skipping nonwritable property " << prop.name();
            continue;
        }

        QByteArray bytes = prop.read(q).toByteArray();
        hasher.addData(bytes);
        
        ++i;
    }
    
    sDebug() << "Changed hash for " << this << " from " << mHash << " to " << hasher.result().toHex();
    sDebug() << "Last changed" << mLastModified << " now " << QDateTime::currentDateTime();
    mLastModified = QDateTime::currentDateTime();
    mHash = hasher.result().toHex();
}

QByteArray SCloudItem::Private::serialize() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    const QMetaObject *myMetaObject = q->metaObject();
    QCryptographicHash hasher(QCryptographicHash::Sha1);

    quint32 propCount = myMetaObject->propertyCount();
    stream << propCount;

    int i = 0;
    while (i < myMetaObject->propertyCount()) {
        QMetaProperty prop = myMetaObject->property(i);

        if (!prop.isReadable()) {
            sDebug() << "Skipping unreadable property" << prop.name();
            continue;
        }
        
        if (!prop.isWritable()) {
            sDebug() << "Skipping nonwritable property " << prop.name();
            continue;
        }

        QVariant propValue = prop.read(q);
        
        stream << QString(prop.name());
        stream << propValue;
        
        ++i;
    }
    
    return data;
}

void SCloudItem::Private::deserialize(QByteArray &bytes, int version)
{
    mDeserialising = true;
    QDataStream stream(&bytes, QIODevice::ReadOnly);
    quint32 propCount;

    stream >> propCount;

    for (quint32 i = 0; i < propCount; ++i) {
        QString propName;
        QVariant propValue;

        stream >> propName;
        stream >> propValue;

        int propIndex = q->metaObject()->indexOfProperty(propName.toUtf8());
        if (propIndex == -1) {
            sDebug() << "Dropping unknown property " << propName;
            continue;
        }

        sDebug() << "Writing to " << propName << propValue;
        q->metaObject()->property(propIndex).write(q, propValue);
    }
    
    mDeserialising = false;
}
