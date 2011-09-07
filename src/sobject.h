/*
 * Copyright (C) 2010-2011 Robin Burchell
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

    /*! Retrieves a list of fields
     *
     * \sa setValue(), value()
     */
    QList<QString> fields() const;

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
