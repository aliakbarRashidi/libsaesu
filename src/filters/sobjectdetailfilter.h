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

#ifndef SOBJECTDETAILFILTER_H
#define SOBJECTDETAILFILTER_H

// Qt
#include <QObject>
#include <QString>
#include <QVariant>

#include "sabstractobjectfilter.h"

class SObjectDetailFilter : public SAbstractObjectFilter
{
public:
    explicit SObjectDetailFilter();
    virtual ~SObjectDetailFilter();

    void setDetailName(const QString &name);
    QString detailName() const;

    void setValue(const QVariant &value);
    QVariant value() const;

    // TODO: support match types other than an exact match

private:
    class Private;
};

#endif // SOBJECTDETAILFILTER_H
