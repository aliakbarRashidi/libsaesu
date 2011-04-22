/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
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

#ifndef SOBJECTREMOVEREQUEST_P_H
#define SOBJECTREMOVEREQUEST_P_H

// Qt
#include <QObject>
#include <QString>

// Us
#include "sabstractobjectrequest_p.h"
#include "sobjectremoverequest.h"

class SObjectRemoveRequest::Private : public SAbstractObjectRequest::Private
{
public:
    explicit Private(QObject *parent);
    virtual ~Private();

    void start(SObjectManager *manager);

    QList<QUuid> mObjectIds;
};

#endif // SOBJECTREMOVEREQUEST_H
