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

#include "epubnetworkmanager.h"
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <epubnetworkreply.h>

EpubNetworkManager::EpubNetworkManager(QObject* parent) :
    QNetworkAccessManager(parent),
    mEpub(nullptr)
{

}

QNetworkReply* EpubNetworkManager::createRequest(Operation op, const QNetworkRequest& request, QIODevice* outgoingData) {
    if (op == QNetworkAccessManager::GetOperation)
        return new EpubNetworkReply(mEpub, request, this);
    else
        return QNetworkAccessManager::createRequest(op, request, outgoingData);
}
