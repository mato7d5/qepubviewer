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
