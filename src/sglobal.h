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

#ifndef SGLOBAL_H
#define SGLOBAL_H

#include <QtCore/QDebug>
#include <QtCore/QDateTime>

#define S_VERIFY(x, y) (!(x) && ((qWarning() << Q_FUNC_INFO << y), Q_ASSERT(x), true))
#define S_VERIFY_OR_DIE(x, y) if (!(x)) { qFatal("%s%s", Q_FUNC_INFO, y); }

#define sDebug() qDebug() << "[debug]" << QDateTime::currentMSecsSinceEpoch() << Q_FUNC_INFO << "-"
#define sWarning() qWarning() << "[warning]" << QDateTime::currentMSecsSinceEpoch() << Q_FUNC_INFO << "-"

#endif // SGLOBAL_H
