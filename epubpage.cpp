#include <QDomDocument>
#include <QDomNodeList>
#include "epubpage.h"

EpubPage::EpubPage(const QString& content)
{
    validateContent(content);
}

void EpubPage::validateContent(const QString& content) {
    QDomDocument dom;
    dom.setContent(content);

    QDomNodeList list = dom.elementsByTagName("img");
    for (int i = 0; i < list.count(); ++i) {
        QString str = list.at(i).attributes().namedItem("src").nodeValue();
        list.at(i).attributes().namedItem("src").setNodeValue("file://" + str);
    }

    list = dom.elementsByTagName("link");
    for (int i = 0; i < list.count(); ++i) {
        if (list.at(i).attributes().namedItem("type").nodeValue() == "text/css") {
            QString str = list.at(i).attributes().namedItem("href").nodeValue();
            list.at(i).attributes().namedItem("href").setNodeValue("file://" + str);
        }
    }

    list = dom.elementsByTagName("a");
    for (int i = 0; i < list.count(); ++i) {
        QString str = list.at(i).attributes().namedItem("href").nodeValue();
        if (str.contains("http://", Qt::CaseInsensitive))
            continue;
        list.at(i).attributes().namedItem("href").setNodeValue("file://" + str);
    }

    mHtmlContent = dom.toString();
}
