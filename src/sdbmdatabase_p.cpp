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

#include <QFileInfo>
#include <QDir>
#include <qmath.h>

#include "sdbmdatabase.h"
#include "sdbmdatabase_p.h"

// TODO: make this adaptive based on the average size of items in this data file
const int BlockSize = 256;

// TODO: size/pos should be quint64
// TODO: error checking

SDBMDatabase::Private::Private(const QString &databaseDirPath)
    : mDatabaseDirPath(databaseDirPath)
{
    QDir dirPath(databaseDirPath);
    if (!dirPath.exists())
        dirPath.mkpath(databaseDirPath);
}

QString SDBMDatabase::Private::pathTo(const QString &fileName) const
{
    return mDatabaseDirPath + QDir::separator() + fileName;
}

void SDBMDatabase::Private::readIndex()
{
    QFileInfo indexInfo(pathTo("db.idx"));
    QDateTime idxLastModified = indexInfo.lastModified();

    if (indexInfo.exists() && idxLastModified != mIndexLastModified) {
        mIndex.clear();

        // reparse index
        QFile indexFile(pathTo("db.idx"));
        indexFile.open(QFile::ReadOnly);
        QDataStream stream(&indexFile);

        stream >> mIndex;

        foreach (const QByteArray &idx, mIndex.keys()) {
            sDebug() << idx.toHex();
        }

        mIndexLastModified = idxLastModified;
    }
}

void SDBMDatabase::Private::writeIndex()
{
    QFile indexFile(pathTo("db.idx"));
    indexFile.open(QFile::WriteOnly);
    QDataStream stream(&indexFile);

    stream << mIndex;
}

bool SDBMDatabase::Private::hasItem(const QByteArray &key)
{
    readIndex();

    return (mIndex.find(key) != mIndex.end());
}

void SDBMDatabase::Private::remove(const QByteArray &key)
{
    readIndex();

    // don't touch the data file, just remove it from the index, and write out the new index.
    // as per the note with set() and padding, we should really have a free list to add the newly
    // freed blocks to.
    //
    // also note, we make sure the item exists to avoid spuriously updating the index.
    if (!hasItem(key))
        return;

    mIndex.remove(key);

    writeIndex();
}

void SDBMDatabase::Private::set(const QByteArray &key, const QByteArray &value)
{
    readIndex();

    // TODO: locking
    int valueBlockSize = qCeil(value.length() / BlockSize) + 1;

    if (hasItem(key)) {
        // try fit the item in the existing space?
        QPair<int, int> ps = *(mIndex.find(key));
        int pos = ps.first;
        int size = ps.second;

        int oldBlockSize = qCeil(size / BlockSize) + 1;

        if (valueBlockSize <= oldBlockSize) {
            // write new value
            QFile dataFile(pathTo("db.dat"));
            dataFile.open(QFile::ReadWrite);
            dataFile.seek(pos);
            dataFile.write(value);
            dataFile.close();
            ps.second = value.size();

            sDebug() << "Wrote " << value.size() << " bytes to " << pos << " for " << key.toHex();

            // update index
            mIndex.insert(key, ps);
            writeIndex();
            return;
        }

        // TODO: if it doesn't fit, we should mark the block as free,
        // so we can try fit other items there in the future.
        sDebug() << "Couldn't fit an update for key " << key.toHex();
    }

    // if we reach this case, either the key isn't in the store yet,
    // or it couldn't fit in the current allocated blocks, so let's chuck it
    // in the end to avoid re-writing the entire store.
    QFile dataFile(pathTo("db.dat"));
    dataFile.open(QFile::ReadWrite);
    sDebug() << "In an append, file is " << dataFile.size() << " bytes";
    dataFile.seek(dataFile.size());

    sDebug() << "Appending at " << dataFile.size() << value.length() << " bytes";

    // before anything, note down the values for the index.
    QPair<int, int> ps;
    ps.first = dataFile.size();
    ps.second = value.size();

    // write the value
    dataFile.write(value);

    // pad the ending of the blocks with null bytes
    QByteArray nulls((valueBlockSize * BlockSize) - value.length(), 0);
    dataFile.write(nulls);

    sDebug() << "Padded with " << nulls.length() << " nulls";

    // update the index
    mIndex.insert(key, ps);
    sDebug() << "Appended item " << key.toHex();
    writeIndex();
}

QByteArray SDBMDatabase::Private::get(const QByteArray &key, bool *ok)
{
    if (!hasItem(key)) {
        *ok = false;
        sDebug() << "No such item: " << key.toHex();
        return QByteArray();
    }

    *ok = true;

    // TODO: LRU caching here could save us a lot of disk IO

    QPair<int, int> ps = *(mIndex.find(key));
    int pos = ps.first;
    int size = ps.second;

    QFile dataFile(pathTo("db.dat"));
    dataFile.open(QFile::ReadOnly);
    dataFile.seek(pos);
    QByteArray data = dataFile.read(size); // TODO: allocate a QBA of size bytes and read into that, checking for error
    dataFile.close();
    sDebug() << "Read item " << key.toHex() << data.length() << " bytes";
    return data;
}
