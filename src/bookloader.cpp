#include "bookloader.h"
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QDebug>

BookLoader::BookLoader(QObject *parent) : QObject(parent)
{
}

QString BookLoader::loadBookContent(const QString &filePath)
{
    if (filePath.endsWith(".fb2", Qt::CaseInsensitive)) {
        return readFB2(filePath);
    } else if (filePath.endsWith(".txt", Qt::CaseInsensitive)) {
        return readTXT(filePath);
    } else {
        return QString();
    }
}

QString BookLoader::readFB2(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }

    QXmlStreamReader xml(&file);

    // Настройки отображения
    int fontSize = 20;
    if (QSysInfo::productType() == "android")
        fontSize *= 1.8;

    QString special;
    QString description;
    QString book;
    QString rId;
    QString rType;
    QString opt;
    QStringList thisToken, content;

    while (!xml.atEnd()) {
        switch (xml.readNext()) {
        case QXmlStreamReader::NoToken:
            qDebug() << "QXmlStreamReader::NoToken";
            break;

        case QXmlStreamReader::StartDocument:
            book = "<!DOCTYPE HTML><html><body style=\"font-size:%1px; font-family:Sans, Times New Roman;\">";
            book = book.arg(fontSize);
            break;

        case QXmlStreamReader::EndDocument:
            book.append("</body></html>");
            break;

        case QXmlStreamReader::StartElement:
            thisToken.append(xml.name().toString());

            if (thisToken.contains("description")) { // ОПИСАНИЕ КНИГИ
                if (thisToken.back() != "image"
                        && thisToken.back() != "first-name"
                        && thisToken.back() != "last-name"
                        && thisToken.back() != "book-title") {
                    break;
                } // не выводим
            }

            if (xml.name().toString() == "title") {
                content.append(""); // добавляем пункт содержания
                break;
            }

            if (xml.name().toString() == "body") {
                if (!xml.attributes().isEmpty() && xml.attributes().first().value().toString() == "notes")
                    special = "notes"; // режим примечаний
            }

            if (special == "notes") {
                if (xml.name().toString() == "section") {
                    if (xml.attributes().count() > 0) {
                        rId = xml.attributes().at(0).value().toString(); // ссылка на текст
                        rType = "";
                    }
                }
            }

            opt = " align=\"justify\"";
            if (thisToken.contains("title") && xml.name().toString() == "p") {
                book.append("<h1>");
                qDebug()<<"-----------------------------------------------------------------Нашел заголовок! "+xml.name().toString();
                if (special == "notes") {
                    opt += (" id=\"" + rId + "\"");
                    qDebug() << "id text" << rId;
                }
            }
            if (thisToken.contains("subtitle")) {
                opt = " align=\"center\" style=\"font-size:" +QString::number(int(fontSize * 1.2)) + "px\" ";
            }
            if (thisToken.contains("annotation")) {
                opt = " align=\"left\" ";
            }

            if (xml.name().toString() == "p" || xml.name().toString() == "subtitle") {
                book.append("<p"+opt +" >");
                break;
            }
            if (xml.name().toString() == "strong" || xml.name().toString() == "sup" || xml.name().toString() == "sub"
                    || xml.name().toString() == "code" || xml.name().toString() == "cite") {
                book.append( "<" + xml.name().toString() + ">");
                break;
            }
            if (xml.name().toString() == "emphasis") {
                book.append( "<i>" );
                break;
            }
            if (xml.name().toString() == "strikethrough") {
                book.append( "<strike>" );
                break;
            }
            if (xml.name().toString() == "author" || xml.name().toString() == "date") {
                book.append( "<p align=\"justify\" style=\"margin-left:45px;\">" );
                break;
            }
            if (xml.name().toString() == "image") { // расположение рисунков
                if (xml.attributes().count() > 0)
                    book.append("<p align=\"center\">"+xml.attributes().at(0).value().toString() + "#" + "</p>");
            }
            if (xml.name() == "binary") { // хранилище рисунков
                if (xml.attributes().at(0).name() == "id") {
                    rId = xml.attributes().at(0).value().toString();
                    rType = xml.attributes().at(1).value().toString();
                }
                if (xml.attributes().at(1).name() == "id") {
                    rId = xml.attributes().at(1).value().toString();
                    rType = xml.attributes().at(0).value().toString();
                }
            }
            break;

        case QXmlStreamReader::EndElement:
            if (thisToken.last() == xml.name().toString()) {
                thisToken.removeLast();
            } else {
                qDebug() << "error token";
            }

            if (xml.name().toString() == "p" || xml.name().toString() == "subtitle" || xml.name().toString() == "strong"
                    || xml.name().toString() == "code" || xml.name().toString() == "cite"
                    || xml.name().toString() == "emphasis" || xml.name().toString() == "sup"
                    || xml.name().toString() == "sub" || xml.name().toString() == "author" || xml.name().toString() == "date") {
                book.append("</p>");
                break;
            }
            if (xml.name().toString() == "title") {
                book.append("</h1>");
                qDebug()<<"-------------------------------------Закрыл заголовок!";
                break;
            }
            if (xml.name().toString() == "td" || xml.name().toString() == "th" || xml.name().toString() == "tr" || xml.name().toString() == "table") {
                book.append( "</"+xml.name().toString()+">" );
                break;
            }
            if (xml.name().toString() == "a") {
                rId.remove("#");
                book.append( "</a><span id=\"" + rId + "___" + "\"></span>" );
                qDebug() << "id" << rId + "___";
                break;
            }
            if (xml.name().toString() == "emphasis") {
                book.append( "</i>" );
                break;
            }
            if (xml.name().toString() == "strikethrough") {
                book.append( "</strike>" );
                break;
            }
            if (special == "notes") { // режим извлечения примечаний
                if (xml.name().toString() == "body") {
                    special = "";
                }
                if (xml.name().toString() == "section") {
                    book.insert(book.lastIndexOf("<"), "<ahref=\"#" + rId + "___" + "\"> назад</a>");
                }
            }
            break;

        case QXmlStreamReader::Characters:
            if (xml.text().toString() == "" || xml.text().toString() == "\n")
                break;

            if (thisToken.contains("description")) { // ОПИСАНИЕ КНИГИ
                description.append(xml.text().toString() + " "); // не выводим
                break;
            }

            if (thisToken.contains( "binary" )) { // для рисунков
                QString image = "<img src=\"data:" + rType + ";base64," + xml.text().toString() + "\"/>";
                book.replace("#"+rId +"#", image);
                rId = "";
                rType = "";
                break;
            }

            if (thisToken.contains("title")) { // формируем содержание
                content.back() += " " + xml.text().toString();
                qDebug() << "title" << xml.text().toString();
            }

            if (special == "notes" && !thisToken.contains("title"))  // добавление текста примечания
                rType += " " + xml.text().toString();

            if (thisToken.contains("p") || thisToken.contains("subtitle") || thisToken.contains("v")
                    || thisToken.contains("emphasis") || thisToken.contains("strong") || thisToken.contains("strikethrough")
                    || thisToken.contains("sup") || thisToken.contains("sub") || thisToken.contains("td")
                    || thisToken.contains("th") || thisToken.contains("code") || thisToken.contains("cite")
                    || thisToken.contains("author") || thisToken.contains("date")) {
                book.append(xml.text().toString());
                break;
            }

            if (special == "notes" && (thisToken.contains("section") || thisToken.contains("body")))
                break;

            if (thisToken.back() == "section" || thisToken.back() == "body") {
                qDebug() << thisToken.back() << xml.text().toString();
                break;
            }

            if (thisToken.back() == "table" || thisToken.back() == "tr" || thisToken.back() == "title"
                    || thisToken.back() == "poem" || thisToken.back() == "stanza")
                break;

            if (thisToken.back() == "a") {
                book.append(xml.text().toString());
                break;
            }

            qDebug() << thisToken.back() <<  "исключение";
            book.append("<span> " + xml.text().toString() + "</span>");
            break;
        }
    }

    file.close();

    return book;
}

QString BookLoader::readTXT(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    return content;
}
