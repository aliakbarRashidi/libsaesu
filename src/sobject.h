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
