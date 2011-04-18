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

#ifndef SOBJECTIDFILTER_H
#define SOBJECTIDFILTER_H

// Qt
#include <QObject>
#include <QString>
#include <QUuid>

#include "sabstractobjectfilter.h"

class SObjectIdFilter : public SAbstractObjectFilter
{
public:
    explicit SObjectIdFilter();
    virtual ~SObjectIdFilter();

    void setIds(const QList<QUuid> &ids);
    QList<QUuid> ids() const;
    void add(const QUuid &id);
    void remove(const QUuid &id);
    void clear();

private:
    class Private;
};

#endif // SOBJECTIDFILTER_H
