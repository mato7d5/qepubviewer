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

#include <QDomDocument>
#include <QDomNodeList>
#include "epubpage.h"

EpubPage::EpubPage(const QString& content)
{
    validateContent(content);
}

void EpubPage::validateContent(const QString& content) {
    QDomDocument dom;
    dom.setContent(content);

    QDomNodeList list = dom.elementsByTagName("img");
    for (int i = 0; i < list.count(); ++i) {
        QString str = list.at(i).attributes().namedItem("src").nodeValue();
        list.at(i).attributes().namedItem("src").setNodeValue("file://" + str);
    }

    list = dom.elementsByTagName("link");
    for (int i = 0; i < list.count(); ++i) {
        if (list.at(i).attributes().namedItem("type").nodeValue() == "text/css") {
            QString str = list.at(i).attributes().namedItem("href").nodeValue();
            list.at(i).attributes().namedItem("href").setNodeValue("file://" + str);
        }
    }

    list = dom.elementsByTagName("a");
    for (int i = 0; i < list.count(); ++i) {
        QString str = list.at(i).attributes().namedItem("href").nodeValue();
        if (str.contains("http://", Qt::CaseInsensitive))
            continue;
        list.at(i).attributes().namedItem("href").setNodeValue("file://" + str);
    }

    mHtmlContent = dom.toString();
}
