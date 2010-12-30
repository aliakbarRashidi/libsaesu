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
#include <QtCore/QUuid>

// Us
#include "sglobal.h"

// XXX:
// THIS IS NOT PUBLIC API.
// It is here for synchronisation to work.
// TODO: find a better way to do this.
struct SCloudItem
{
    // TODO: make hash look up a SCloudItem type, SCloudItem should contain field timestamps + hashes
    QUuid mUuid;
    quint64 mTimeStamp;
    QByteArray mHash;
    QHash<QString, QVariant> mFields;
};

QDataStream &operator<<(QDataStream &out, const SCloudItem &item);
QDataStream &operator>>(QDataStream &in, SCloudItem &item);
QDebug operator<<(QDebug dbg, const SCloudItem &item3);
// END HACK


class SCloudStorage : public QObject
{
    Q_OBJECT
public:
    static SCloudStorage *instance(const QString &cloudName);

    void load();
    void save();

    bool hasItem(const QUuid &uuid) const;
    SCloudItem *item(const QUuid &uuid) const;
    void insertItem(SCloudItem *item);

    QVariant get(const QUuid &uuid, const QString &field) const;
    void set(const QUuid &uuid, const QString &field, const QVariant &data);
    const QUuid &create();
    void destroy(const QUuid &uuid);

    // for sync stuff
    const QByteArray &hash(const QUuid &uuid);
    quint64 modifiedAt(const QUuid &uuid);

    QList<QUuid> itemUUIDs() const;

signals:
    void changed(const QUuid &uuid, const QString &fieldName);
    void created(const QUuid &uuid);
    void destroyed(const QUuid &uuid);

private:
    SCloudStorage(const QString &cloudName, QObject *parent = 0);
    virtual ~SCloudStorage();

    class Private;
    Private *d;
};

#endif // SCLOUDSTORAGE_H
