#ifndef BOOKDATABASEMANAGER_H
#define BOOKDATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class BookDatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit BookDatabaseManager(QObject *parent = nullptr);

    Q_INVOKABLE void addBook(const QString &title, const QString &author, const QString &filePath);
    Q_INVOKABLE void delBook(int id);
    Q_INVOKABLE QVariantList loadBooks();

signals:
    void bookAdded(const QVariantMap& book);
    void bookRemoved(int bookId);


};

#endif // BOOKDATABASEMANAGER_H
