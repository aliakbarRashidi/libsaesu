/*
 * Copyright (C) 2010 Robin Burchell <robin.burchell@collabora.co.uk>
 * Copyright (C) 2010 John Brooks <john.brooks@dereferenced.net>
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

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <qmath.h>

#include "sdbmdatabase.h"
#include "sdbmdatabase_p.h"

// TODO: make this adaptive based on the average size of items in this data file
const int BlockSize = 256;

// TODO: size/pos should be quint64
// TODO: error checking

// locking includes
#ifdef Q_OS_UNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#else
#error "Port me please"
#endif

class SDBMDatabaseLockFile
{
private:
    bool m_locked;
    QString m_path;
    int fd;
public:
    SDBMDatabaseLockFile(const QString &p)
        : m_locked(false)
        , m_path(p)
    {
        acquire();
    }

    ~SDBMDatabaseLockFile()
    {
        release();
    }

    QString lockFile() const
    {
        return QDir::toNativeSeparators(m_path);
    }

    bool isLocked() const
    {
        return m_locked;
    }

#ifdef Q_OS_UNIX
    int acquire()
    {
        if (isLocked()) {
            sDebug() << "Already locked!";
            return 1;
        }

        // try acquire a lock forever and ever
        int tries = 0;
        forever {
            fd = open(lockFile().toLocal8Bit().constData(), O_RDWR | O_CREAT | O_TRUNC, 0600);
            if (fd < 0) {
                sDebug() << "Open failed: " << errno;
                return -1;
            }

            struct flock lock;
            memset(&lock, 0, sizeof(lock));
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_SET;
            if (fcntl(fd, F_SETLK, &lock) < 0) {
                int err = errno;
                close(fd);
                if (err != EACCES && err != EAGAIN) {
                    sDebug() << "UNKNOWN Error locking: " << errno;

                    // we might want to return here, but then again, bad idea
                    // if we continue without acquiring a lock, then we're subject to all sorts of odd races
                    // i'd rather have an (easily found) infinite loop -> crash here rather than races.
                    // return -1;
                }

                close(fd);

                // try to get the lock again
                QCoreApplication::processEvents();

                tries++;

                // in heavy usage conditions, we can actually take a while to acquire the lock
                // so as to not panic unnecessarily, let's sleep a little here
                // TODO: this is very very bad, we shouldn't sleep the main thread
                // this will require architecting to fix, though: move all access
                // (and locking) to a thread, communicate changes to db state via signals
                usleep(1);

                if (tries == 50) {
                    sDebug() << "Tried 50 times to lock! What's going on?";
                    usleep(50);
                } else if (tries == 500) {
                    qCritical("Can't acquire a lock, life is hard");
                }
            } else {
                m_locked = true;
                return 1;
            }
        }
    }

    void release()
    {
        if (!isLocked())
            return;

        close(fd);

        /* Attempt to delete the lock file; it doesn't really matter if this fails */
        unlink(lockFile().toLocal8Bit().constData());
    }
#else
#error "port me please"
#endif
};

SDBMDatabase::Private::Private(const QString &databaseDirPath)
    : mDatabaseDirPath(databaseDirPath)
{
    QDir dirPath(databaseDirPath);
    if (!dirPath.exists())
        dirPath.mkpath(databaseDirPath);

    readIndex();
    writeIndex(); // this is a cheat's way of creating the file so QFSWatcher is happy
    connect(&mFsWatcher, SIGNAL(fileChanged(QString)), SLOT(onIndexChanged()));
    mFsWatcher.addPath(pathTo("db.idx"));
}

void SDBMDatabase::Private::onIndexChanged()
{
    readIndex();
}

QString SDBMDatabase::Private::pathTo(const QString &fileName) const
{
    return mDatabaseDirPath + QDir::separator() + fileName;
}

void SDBMDatabase::Private::readIndex()
{
    SDBMDatabaseLockFile lf(pathTo("db.lock"));
    sDebug() << "Reading index";
    mIndex.clear();
    mKeys.clear();

    // reparse index
    QFile indexFile(pathTo("db.idx"));
    indexFile.open(QFile::ReadOnly);
    QDataStream stream(&indexFile);

    stream >> mIndex;

    foreach (const QByteArray &key, mIndex.keys())
        mKeys.append(key);

    sDebug() << "Read index of " << mIndex.count() << " items";
}

void SDBMDatabase::Private::writeIndex()
{
    SDBMDatabaseLockFile lf(pathTo("db.lock"));
    sDebug() << "Writing index";
    QFile indexFile(pathTo("db.idx"));
    indexFile.open(QFile::WriteOnly);
    QDataStream stream(&indexFile);
    stream << mIndex;
    sDebug() << "Index written";
}

bool SDBMDatabase::Private::hasItem(const QByteArray &key)
{
    return (mIndex.find(key) != mIndex.end());
}

void SDBMDatabase::Private::remove(const QByteArray &key)
{
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
    SDBMDatabaseLockFile lf(pathTo("db.lock"));

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
    dataFile.seek(dataFile.size());

    sDebug() << "Appending at " << dataFile.size() << value.length() << " bytes for " << key.toHex();

    // before anything, note down the values for the index.
    QPair<int, int> ps;
    ps.first = dataFile.size();
    ps.second = value.size();

    // write the value
    dataFile.write(value);

    // pad the ending of the blocks with null bytes
    QByteArray nulls((valueBlockSize * BlockSize) - value.length(), 0);
    dataFile.write(nulls);

    // update the index
    mIndex.insert(key, ps);
    writeIndex();
}

QByteArray SDBMDatabase::Private::get(const QByteArray &key, bool *ok)
{
    SDBMDatabaseLockFile lf(pathTo("db.lock"));

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
    return data;
}

const QList<QByteArray> &SDBMDatabase::Private::keys() const
{
    return mKeys;
}
