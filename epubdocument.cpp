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

EpubDocument::~EpubDocument() {
    if (mEpub)
        epub_close(mEpub);

    epub_cleanup();
}
