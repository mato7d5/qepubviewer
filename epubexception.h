#ifndef EPUBEXCEPTION_H
#define EPUBEXCEPTION_H

#include <QString>
#include <QException>

class EpubException : public QException
{
private:
    QString message;

public:
    EpubException(const QString& msg);

    const char* what() const noexcept { return message.toStdString().c_str(); }

};

#endif // EPUBEXCEPTION_H
