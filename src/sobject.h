/*
 * Copyright (C) 2011 Robin Burchell <robin.burchell@collabora.co.uk>
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

#ifndef SOBJECT_H
#define SOBJECT_H

// Qt
#include <QVariant>
#include <QUuid>
#include <QSharedDataPointer>

#include "sobjectid.h"

/*! SObject represents a row in a data store.
 */
class SObject
{
public:
    SObject();
    SObject(const SObject &);
    SObject &operator=(const SObject &);
    ~SObject();

    QByteArray hash() const;
    qint64 lastSaved() const;

    SObjectId id() const;
    void setId(const SObjectId &id);

    /*! Retrieves data for a given \a fieldName.
     */
    QVariant value(const QString &fieldName) const;

    /*! Sets a \a fieldName to a given \a value.
     */
    void setValue(const QString &fieldName, const QVariant &value);

private:
    friend QDataStream &operator<<(QDataStream &out, const SObject &item);
    friend QDataStream &operator>>(QDataStream &in, SObject &item);
    friend class SObjectSaveRequest;
    friend class SObjectFetchRequest;

    class Private;
    QSharedDataPointer<Private> d;
};

QDataStream &operator<<(QDataStream &out, const SObject &item);
QDataStream &operator>>(QDataStream &in, SObject &item);

#endif // SOBJECT_H
