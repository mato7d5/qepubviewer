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

#ifndef EPUBNETWORKMANAGER_H
#define EPUBNETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <epub.h>

class EpubNetworkManager : public QNetworkAccessManager
{
//    Q_OBJECT

private:
    epub* mEpub;

public:
    EpubNetworkManager(QObject* parent = nullptr);

    virtual QNetworkReply* createRequest(Operation op, const QNetworkRequest& request, QIODevice* outgoingData);

    void setEpub(epub* epubdata) noexcept { mEpub = epubdata; }
    void resetEpub() noexcept { mEpub = nullptr; }
};

#endif // EPUBNETWORKMANAGER_H
