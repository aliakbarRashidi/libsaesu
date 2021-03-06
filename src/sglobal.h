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

#ifndef SGLOBAL_H
#define SGLOBAL_H

#include <QtCore/QDebug>
#include <QtCore/QDateTime>

#define S_VERIFY(x, y) (!(x) && ((qWarning() << Q_FUNC_INFO << y), Q_ASSERT(x), true))
#define S_VERIFY_OR_DIE(x, y) if (!(x)) { qFatal("%s%s", Q_FUNC_INFO, y); }

#define sDebug() qDebug() << "[debug]" << QDateTime::currentMSecsSinceEpoch() << Q_FUNC_INFO << "-"
#define sWarning() qWarning() << "[warning]" << QDateTime::currentMSecsSinceEpoch() << Q_FUNC_INFO << "-"

#endif // SGLOBAL_H
