/***************************************************************************
 *   Copyright (C) 2015 - 2017 by Martin Mancuska                          *
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

#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H

#include <QObject>
#include <QString>
#include <QList>
#include <epub.h>
#include <map>
#include <vector>
#include "epubpage.h"

struct ContentMapValue{
    QString title;
    QString url;
};

using ContentMap = std::vector<std::pair<unsigned int, ContentMapValue> >;

class EpubDocument : public QObject
{
    Q_OBJECT
private:
    epub* mEpub;
    QString mFileName;
    QList<EpubPage> mPages;
    std::map<QString, int> mPageUrlMap;
    ContentMap mContentMap;

    void extractPages();
    void extractContent();

public:
    explicit EpubDocument(const QString fileName, QObject *parent = 0);
    ~EpubDocument();

    bool isEmpty() const { return mPages.isEmpty(); }
    int numOfPages() const { return mPages.size(); }
    const EpubPage& page(int index) { return mPages[index]; }
    epub* epubdata() const { return mEpub; }
    const std::map<QString, int>& pageUrlMap() const { return mPageUrlMap; }
    const ContentMap& contentMap() const { return mContentMap; }

signals:

public slots:

};

#endif // EPUBDOCUMENT_H
