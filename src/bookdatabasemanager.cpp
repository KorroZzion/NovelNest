#include "bookdatabasemanager.h"
#include <QStandardPaths>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

BookDatabaseManager::BookDatabaseManager(QObject *parent) : QObject(parent)
{
    /*
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/books.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
*/
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bookss.db");

    if (!db.open()) {
        qDebug() << "Error: unable to open database";
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS books (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, author TEXT, filePath TEXT)");

}

void BookDatabaseManager::addBook(const QString &title, const QString &author, const QString &filePath) {
    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, filePath) VALUES (?, ?, ?)");
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(filePath);
    if (!query.exec()) {
        qDebug() << "Error adding book: " << query.lastError();
    } else {
        int id = query.lastInsertId().toInt();
        qDebug() << "Book added successfully with ID:" << id;
        emit bookAdded(QVariantMap{{"id", id}, {"title", title}, {"author", author}, {"filePath", filePath}});
    }
}

void BookDatabaseManager::delBook(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Error deleting book: " << query.lastError();
    } else {
        qDebug() << "Book deleted successfully with id:" << id;
        emit bookRemoved(id);
    };
}


QVariantList BookDatabaseManager::loadBooks() {
    QVariantList bookList;
    QSqlQuery query("SELECT id, title, author, filePath FROM books");

    while (query.next()) {
        QVariantMap book;
        book["id"] = query.value(0).toInt();
        book["title"] = query.value(1).toString();
        book["author"] = query.value(2).toString();
        book["filePath"] = query.value(3).toString();
        bookList.append(book);
    }
    return bookList;
}

