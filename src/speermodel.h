/*
 * Copyright (C) 2010-2011 Robin Burchell
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SPEERMODEL_H
#define SPEERMODEL_H

// Qt
#include <QAbstractItemModel>
#include <QString>

// Us
#include "sglobal.h"
class SPeerModelPrivate;

class SPeerModel : public QObject
{
public:
    explicit SPeerModel(QObject *parent);
    virtual ~SPeerModel();

private:
    SPeerModelPrivate *d;
};

#endif // SPEERMODEL_H
