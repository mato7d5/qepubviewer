#include "epubnetworkreply.h"
#include <QFileInfo>
#include <QString>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QMutexLocker>
#include <cstring>

QMutex EpubNetworkReply::sMutex;

EpubNetworkReply::EpubNetworkReply(epub* epubdata, const QNetworkRequest& request, QObject* parent) :
    QNetworkReply(parent),
    mEpub(epubdata),
    mUrl(request.url()),
    mPosition(0)
{
    setRequest(request);

    setUrl(mUrl);
    setHeader(QNetworkRequest::ContentTypeHeader, fileType());

    open(ReadOnly | Unbuffered);

    connect(&mWatcher, SIGNAL(finished()), this, SLOT(epubContentDone()));
    QFuture<QByteArray> future = QtConcurrent::run(EpubNetworkReply::readEpubContent, mEpub, mUrl);
    mWatcher.setFuture(future);
}

QVariant EpubNetworkReply::fileType() {
    QString path = mUrl.host() + mUrl.path();
    if (path.endsWith("/"))
        path.remove(path.length() -1 , 1);

    QString suffix = QFileInfo(path).suffix();

    if (suffix == "jpg" || suffix == "jpeg")
        return QVariant("image/jpeg");
    else if (suffix == "png")
        return QVariant("image/png");
    else if (suffix == "css")
        return QVariant("text/css");

    return QVariant();
}

QByteArray EpubNetworkReply::readEpubContent(epub *epubdata, const QUrl& url) {
    // seems ebub is not thread safety
    QMutexLocker locker(&sMutex);

    QString fileName = url.host() + url.path();

    if (fileName.endsWith("/"))
        fileName.remove(fileName.length() - 1, 1);

    char *buffer = nullptr;
    int bufSize = epub_get_data(epubdata, fileName.toUtf8(), &buffer);

    QByteArray ret(buffer, bufSize);

    if (buffer)
        free(buffer);

    return ret;
}

void EpubNetworkReply::epubContentDone() {
    if (mWatcher.isFinished()) {
        mPosition = 0;
        mByteBuffer = mWatcher.result();

        emit readyRead();
        emit finished();
    }
}

qint64 EpubNetworkReply::readData(char* data, qint64 size) {
    qint64 readSize = qMin(size, static_cast<qint64> (mByteBuffer.size() - mPosition));

    memcpy(data, mByteBuffer.constData() + mPosition, readSize);
    mPosition += readSize;

    return readSize;
}


bool EpubNetworkReply::seek(qint64 pos) {
    if (pos < 0 || pos > mByteBuffer.size())
        return false;

    mPosition = pos;
    return true;
}
