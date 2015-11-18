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

#ifndef EPUBPAGE_H
#define EPUBPAGE_H

#include <QString>

class EpubPage
{
private:
    QString mHtmlContent;

    void validateContent(const QString& content);

public:
    explicit EpubPage(const QString& content = QString());

    void setHtmlContent(const QString& content) { mHtmlContent = content; }
    QString htmlContent() const noexcept { return mHtmlContent; }
 };

#endif // EPUBPAGE_H
