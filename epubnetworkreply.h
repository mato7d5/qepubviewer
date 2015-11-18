/***************************************************************************
 *   Copyright (C) 2015 by Martin Mancuska                                 *
 *   <martin@borg.sk>                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef EPUBNETWORKREPLY_H
#define EPUBNETWORKREPLY_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QVariant>
#include <QByteArray>
#include <QFutureWatcher>
#include <QMutex>
#include <epub.h>

class EpubNetworkReply : public QNetworkReply
{
    Q_OBJECT
private:
    epub* mEpub;
    QUrl mUrl;
    QByteArray mByteBuffer;
    QFutureWatcher<QByteArray> mWatcher;
    qint64 mPosition;

    QVariant fileType();

    static QByteArray readEpubContent(epub* epubdata, const QUrl& url);
    static QMutex sMutex;

public slots:
    void epubContentDone();

public:
    EpubNetworkReply(epub* epubdata, const QNetworkRequest& request, QObject* parent = nullptr);

    qint64 readData(char* data, qint64 size);
    virtual qint64 bytesAvailable() const { return mByteBuffer.size() - mPosition; }
    virtual qint64 pos() const { return mPosition; }
    virtual bool seek(qint64 pos);
    virtual qint64 size() const { return mByteBuffer.size(); }
    virtual void abort() { }
};

#endif // EPUBNETWORKREPLY_H
