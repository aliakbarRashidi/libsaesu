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

// Qt
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>

// Us
#include "sobjectsaverequest.h"
#include "sobjectsaverequest_p.h"
#include "sobject.h"

SObjectSaveRequest::SObjectSaveRequest(QObject *parent)
    : SAbstractObjectRequest(parent)
{
    d = new Private;
}

SObjectSaveRequest::~SObjectSaveRequest()
{
    delete d;
}

void SObjectSaveRequest::setObjects(const QList<SObject> &objects)
{
    static_cast<Private*>(d)->mObjects = objects;
}

void SObjectSaveRequest::setObject(const SObject &object)
{
    static_cast<Private*>(d)->mObjects = QList<SObject>() << object;
}

QList<SObject> SObjectSaveRequest::objects() const
{
    return static_cast<Private*>(d)->mObjects;
}
