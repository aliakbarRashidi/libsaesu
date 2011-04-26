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
#include "sobjectdetailfilter.h"
#include "sobjectdetailfilter_p.h"

SObjectDetailFilter::SObjectDetailFilter()
     : SAbstractObjectFilter()
{
    d = QSharedPointer<SAbstractObjectFilter::Private>(new SObjectDetailFilter::Private);
}

SObjectDetailFilter::~SObjectDetailFilter()
{
}

void SObjectDetailFilter::setDetailName(const QString &name)
{
    static_cast<Private*>(d.data())->mDetailName = name;
}

QString SObjectDetailFilter::detailName() const
{
    return static_cast<Private*>(const_cast<SAbstractObjectFilter::Private*>(d.data()))->mDetailName;
}

void SObjectDetailFilter::setValue(const QVariant &value)
{
    static_cast<Private*>(d.data())->mDetailValue = value;
}

QVariant SObjectDetailFilter::value() const
{
    return static_cast<Private*>(const_cast<SAbstractObjectFilter::Private*>(d.data()))->mDetailValue;
}
