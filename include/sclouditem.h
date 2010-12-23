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

#ifndef SCLOUDITEM_H
#define SCLOUDITEM_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QUuid>
#include <QtCore/QDateTime>

// Us
#include "sglobal.h"

class SCloudItem : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE SCloudItem(const QString &cloudName);
    virtual ~SCloudItem();

    Q_PROPERTY(QString uuid READ uuid WRITE setUuid)
    const QString &uuid() const;
    Q_PROPERTY(QDateTime lastModified READ lastModified WRITE setLastModified)
    const QDateTime &lastModified() const;
    Q_PROPERTY(QByteArray hash READ hash WRITE setHash)
    const QByteArray &hash() const;

    QByteArray serialize() const;
    void deserialize(QByteArray &bytes, int version);

protected:
    void objectModified();

private:
    void setUuid(const QString &uuid);
    void setLastModified(const QDateTime &lastModified);
    void setHash(const QByteArray &hash);

    class Private;
    Private *d;
};

#endif // SCLOUDITEM_H
