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
