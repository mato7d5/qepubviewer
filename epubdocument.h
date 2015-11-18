#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H

#include <QObject>
#include <QString>
#include <QList>
#include <epub.h>
#include <map>
#include "epubpage.h"

class EpubDocument : public QObject
{
    Q_OBJECT
private:
    epub* mEpub;
    QString mFileName;
    QList<EpubPage> mPages;
    std::map<QString, int> mPageUrlMap;

public:
    explicit EpubDocument(const QString fileName, QObject *parent = 0);
    ~EpubDocument();

    bool isEmpty() const { return mPages.isEmpty(); }
    int numOfPages() const { return mPages.size(); }
    const EpubPage& page(int index) { return mPages[index]; }
    epub* epubdata() const { return mEpub; }
    const std::map<QString, int>& pageUrlMap() const { return mPageUrlMap; }

signals:

public slots:

};

#endif // EPUBDOCUMENT_H
