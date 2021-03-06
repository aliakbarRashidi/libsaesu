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
