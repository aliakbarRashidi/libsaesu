#ifndef SGLOBAL_H
#define SGLOBAL_H

#define S_VERIFY(x, y) (!(x) && ((qWarning() << Q_FUNC_INFO << y), Q_ASSERT(x), true))
#define S_VERIFY_OR_DIE(x, y) if (!(x)) { qFatal("%s%s", Q_FUNC_INFO, y); }

#endif // SGLOBAL_H
