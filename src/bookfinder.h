#ifndef BOOKFINDER_H
#define BOOKFINDER_H

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QVariantMap>
#include <QVariant>


class BookFinder : public QObject
{
    Q_OBJECT
public:
    explicit BookFinder(QObject *parent = nullptr);

    Q_INVOKABLE void findFb2Books(const QVariantList &existingBooks);
    Q_INVOKABLE QVariantMap extractBookInfo(const QString &filePath);

signals:
    void booksFound(const QStringList &books);

private:
    void scanDirectory(const QDir &directory, QStringList &bookList, const QStringList &existingPaths);


};

#endif // BOOKFINDER_H
