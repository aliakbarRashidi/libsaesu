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

#ifndef SOBJECTFETCHREQUEST_P_H
#define SOBJECTFETCHREQUEST_P_H

// Qt
#include <QObject>

// Us
#include "sglobal.h"
#include "sobject.h"
#include "sobjectfetchrequest.h"
#include "sabstractobjectrequest_p.h"

class SObjectFetchRequest::Private : public SAbstractObjectRequest::Private
{
    Q_OBJECT
public:
    explicit Private(QObject *parent = 0);

    void run();

    QList<SObject> mObjects;
};

#endif // SOBJECTFETCHREQUEST_P_H
