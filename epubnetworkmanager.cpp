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
