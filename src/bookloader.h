#ifndef BOOKLOADER_H
#define BOOKLOADER_H

#include <QObject>

class BookLoader : public QObject
{
    Q_OBJECT
public:
    explicit BookLoader(QObject *parent = nullptr);

    Q_INVOKABLE QString loadBookContent(const QString &filePath);

signals:

private:
    QString readFB2(const QString &filePath);
    QString readTXT(const QString &filePath);

};

#endif // BOOKLOADER_H
