#include "bookfinder.h"
#include <QStandardPaths>
#include <QXmlStreamReader>
#include <QFile>

BookFinder::BookFinder(QObject *parent) : QObject(parent)
{

}

void  BookFinder::findFb2Books(const QVariantList &existingBooks)
{
    QStringList bookList;
    QStringList existingPaths;
    for (const QVariant &book : existingBooks) {
        QVariantMap bookMap = book.toMap();
        existingPaths.append(bookMap["filePath"].toString());
    }

    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QDir homeDirectory(homePath);

    if (homeDirectory.exists()) {
        scanDirectory(homeDirectory, bookList, existingPaths);
    }

    emit booksFound(bookList);
}

void BookFinder::scanDirectory(const QDir &directory, QStringList &bookList, const QStringList &existingPaths)
{
    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &fileInfo : fileInfoList) {
        if (fileInfo.isDir()) {
            scanDirectory(QDir(fileInfo.absoluteFilePath()), bookList, existingPaths);
        } else {
            QString suffix = fileInfo.suffix().toLower();
            if (suffix == "fb2" || suffix == "epub" || suffix == "txt") {
                QString filePath = fileInfo.absoluteFilePath();
                if (!existingPaths.contains(filePath)) {
                    QString fileName = fileInfo.completeBaseName();
                    bookList.append(fileName + "::" + filePath);
                }
            }
        }
    }
}

QVariantMap BookFinder::extractBookInfo(const QString &filePath) // Добавить под epub
{
    QVariantMap bookInfo;
    QFile file(filePath);

    QFileInfo fileInfo(filePath);
    QString fileExtension = fileInfo.suffix().toLower();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return bookInfo;
    }

    if (fileExtension == "fb2") {
        QXmlStreamReader xml(&file);
        QString title, author;

        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();

            if (xml.isStartElement()) {
                if (xml.name() == "book-title") {
                    title = xml.readElementText();
                } else if (xml.name() == "author") {
                    while (!(xml.isEndElement() && xml.name() == "author")) {
                        xml.readNext();

                        if (xml.isStartElement() && xml.name() == "first-name") {
                            author = xml.readElementText();
                        } else if (xml.isStartElement() && xml.name() == "last-name") {
                            if (!author.isEmpty()) {
                                author += " ";
                            }
                            author += xml.readElementText();
                        }
                    }
                }
            }
        }

        bookInfo["title"] = title;
        bookInfo["author"] = author;
    } else if (fileExtension == "txt") {
        QString title = fileInfo.completeBaseName();
        bookInfo["title"] = title;
        bookInfo["author"] = "";
    }

    return bookInfo;
}
