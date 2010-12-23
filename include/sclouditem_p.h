/*
 * Copyright (C) 2010 Robin Burchell <robin.burchell@collabora.co.uk>
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

#ifndef SCLOUDITEM_P_H
#define SCLOUDITEM_P_H

// Us
class SCloudItem;

class SCloudItem::Private
{
public:
    Private(const QString &cloudName, SCloudItem *p);
    virtual ~Private();
    
    void objectModified();
    QByteArray serialize() const;
    void deserialize(QByteArray &bytes, int version);

    SCloudItem *q;
    QString mCloudName;
    QString mUuid;
    QByteArray mHash;
    QDateTime mLastModified;
    bool mDeserialising;
};

#endif // SCLOUDITEM_P_H
