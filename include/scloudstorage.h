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
    static QString cloudPath(const QString &cloudName);

    const QString &cloudName() const;
    bool hasItem(const QByteArray &uuid) const;
    QVariant get(const QByteArray &uuid, const QString &field) const;
    void set(const QByteArray &uuid, const QString &field, const QVariant &data);
    QByteArray create();
    void destroy(const QByteArray &uuid);

    // for sync stuff
    QList<QByteArray> itemUUIDs() const;
    SCloudItem *item(const QByteArray &uuid) const;
    void insertItem(const QByteArray &uuid, SCloudItem *item);

signals:
    void changed(const QByteArray &uuid);
    void created(const QByteArray &uuid);
    void destroyed(const QByteArray &uuid);

private:
    SCloudStorage(const QString &cloudName, QObject *parent = 0);
    virtual ~SCloudStorage();

    class Private;
    Private *d;
};

#endif // SCLOUDSTORAGE_H
