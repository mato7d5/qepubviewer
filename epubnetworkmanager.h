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
