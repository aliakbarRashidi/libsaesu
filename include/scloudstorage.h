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

#ifndef SCLOUDSTORAGE_H
#define SCLOUDSTORAGE_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QVariantMap>

// Us
#include "sglobal.h"
class SCloudItem;

class SCloudStorage : public QObject
{
    Q_OBJECT
public:
    static SCloudStorage *instance(const QString &cloudName);

    void load();
    void save();

    QVariant get(const QString &uuid, const QString &field) const;
    void set(const QString &uuid, const QString &field, const QVariant &data);
    QString create();
    void destroy(const QString &uuid);

    // for sync stuff
    const QByteArray &hash(const QString &uuid);
    quint64 modifiedAt(const QString &uuid);

    QList<QString> itemUUIDs() const;

signals:
    void changed(const QString &uuid, const QString &fieldName);
    void created(const QString &uuid);
    void destroyed(const QString &uuid);

private:
    SCloudStorage(const QString &cloudName, QObject *parent = 0);
    virtual ~SCloudStorage();

    class Private;
    Private *d;
};

#endif // SCLOUDSTORAGE_H
