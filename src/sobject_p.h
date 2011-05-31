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
