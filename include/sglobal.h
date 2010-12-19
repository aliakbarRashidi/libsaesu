#ifndef SGLOBAL_H
#define SGLOBAL_H

#include <QtCore/QDebug>

#define S_VERIFY(x, y) (!(x) && ((qWarning() << Q_FUNC_INFO << y), Q_ASSERT(x), true))
#define S_VERIFY_OR_DIE(x, y) if (!(x)) { qFatal("%s%s", Q_FUNC_INFO, y); }

#define sDebug() qDebug() << "[debug]" << Q_FUNC_INFO << "-"
#define sWarning() qWarning() << "[warning]" << Q_FUNC_INFO << "-"

#endif // SGLOBAL_H
