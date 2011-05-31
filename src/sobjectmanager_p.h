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

#ifndef SOBJECTMANAGER_P_H
#define SOBJECTMANAGER_P_H

// Qt
#include <QObject>
#include <QString>
#include <QSqlDatabase>

// Us
#include "sobjectmanager.h"
#include "sipcchannel.h"

class SObjectManager::Private : public QObject
{
    Q_OBJECT
public:
    explicit Private(const QString &tableName, QObject *parent = 0);
    
    QSqlDatabase connection();

    QSqlDatabase mConnection;

    QString mTableName;

    SIpcChannel mIpcChannel;

private slots:
    void onIpcMessage(const QString &message, const QByteArray &data);
};

#endif // SOBJECTMANAGER_P_H
