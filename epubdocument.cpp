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

#include "epubdocument.h"
#include <epub.h>
#include "epubexception.h"

EpubDocument::EpubDocument(const QString fileName, QObject *parent) :
    QObject(parent),
    mFileName(fileName)
{
    mEpub = epub_open(mFileName.toStdString().c_str(), 0);
    if (mEpub == nullptr)
        throw EpubException(QString("Cannot get epub information about %1").arg(mFileName));

    extractPages();
    extractContent();
}

EpubDocument::~EpubDocument() {
    if (mEpub)
        epub_close(mEpub);

    epub_cleanup();
}

void EpubDocument::extractPages()
{
    eiterator* it = epub_get_iterator(mEpub, EITERATOR_SPINE, 0);

    QString pageContent;
    QString contentUrl;
    int pageNum = 1;

    do {
        if (epub_it_get_curr(it)) {
            QString line = epub_it_get_curr(it);
            if (line.contains("</html>", Qt::CaseInsensitive)) {
                pageContent += line;

                EpubPage page(pageContent);

                contentUrl = epub_it_get_curr_url(it);
                mPageUrlMap[contentUrl] = pageNum;

                mPages.push_back(page);
                pageContent.clear();
                contentUrl.clear();
                ++pageNum;
            }
        }
    } while (epub_it_get_next(it));

    epub_free_iterator(it);
}

void EpubDocument::extractContent() {
    titerator* tit = epub_get_titerator(mEpub, TITERATOR_NAVMAP, 0);

    if (tit != nullptr) {
        do {
            QString title = epub_tit_get_curr_label(tit);
            QString url = epub_tit_get_curr_link(tit);

            std::pair<QString, QString> item(title, url);
            mContentMap.push_back(item);

        } while (epub_tit_next(tit));

        epub_free_titerator(tit);
    }
}
