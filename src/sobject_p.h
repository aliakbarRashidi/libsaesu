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

#ifndef SOBJECT_SOBJECT_P_H
#define SOBJECT_SOBJECT_P_H

// Qt
#include <QSharedData>
#include <QHash>
#include <QString>
#include <QUuid>

// Us
#include "sobject.h"

class SObject::Private: public QSharedData
{
public:
    explicit Private();

    QHash<QString, QVariant> mValues;
    SObjectId mId;
    qint64 mLastSaved;
    QByteArray mHash;
};

#endif // SOBJECT_SOBJECT_P_H