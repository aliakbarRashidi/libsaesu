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

// Qt
#include <QSharedData>

// Us
#include "sobjectidfilter.h"
#include "sobjectidfilter_p.h"

SObjectIdFilter::SObjectIdFilter()
     : SAbstractObjectFilter()
{
    d = QSharedPointer<SAbstractObjectFilter::Private>(new SObjectIdFilter::Private());
}

SObjectIdFilter::~SObjectIdFilter()
{
}

void SObjectIdFilter::setIds(const QList<SObjectLocalId> &ids)
{
    static_cast<Private*>(d.data())->mFilterIds = ids;
}

QList<SObjectLocalId> SObjectIdFilter::ids() const
{
    return static_cast<Private*>(const_cast<SAbstractObjectFilter::Private*>(d.data()))->mFilterIds;
}

void SObjectIdFilter::add(const SObjectLocalId &id)
{
    static_cast<Private*>(d.data())->mFilterIds.append(id);
}

void SObjectIdFilter::remove(const SObjectLocalId &id)
{
    static_cast<Private*>(d.data())->mFilterIds.removeAll(id);
}

void SObjectIdFilter::clear()
{
    static_cast<Private*>(d.data())->mFilterIds.clear();
}

