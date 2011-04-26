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

#ifndef SGLOBAL_H
#define SGLOBAL_H

#include <QtCore/QDebug>
#include <QtCore/QDateTime>

#define S_VERIFY(x, y) (!(x) && ((qWarning() << Q_FUNC_INFO << y), Q_ASSERT(x), true))
#define S_VERIFY_OR_DIE(x, y) if (!(x)) { qFatal("%s%s", Q_FUNC_INFO, y); }

#define sDebug() qDebug() << "[debug]" << QDateTime::currentMSecsSinceEpoch() << Q_FUNC_INFO << "-"
#define sWarning() qWarning() << "[warning]" << QDateTime::currentMSecsSinceEpoch() << Q_FUNC_INFO << "-"

#endif // SGLOBAL_H
