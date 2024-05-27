#include <auroraapp/auroraapp.h>
#include <QtQuick>

#include <QtQml>
#include "bookdatabasemanager.h"
#include "bookfinder.h"
#include "booksmodel.h"
#include "bookloader.h"



int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.asdnp"));
    application->setApplicationName(QStringLiteral("NovelNest"));


    qmlRegisterType<BooksModel>("BooksModel", 1, 0, "BooksModel");
    qmlRegisterType<BookLoader>("BookLoader", 1, 0, "BookLoader");
    //qmlRegisterType<BookDatabaseManager>("BookDatabaseManager", 1, 0, "BookDatabaseManager");

    QScopedPointer<QQuickView> view(Aurora::Application::createView());


    auto bookFinder = new BookFinder(application.data());
    //auto bookLoader = new BookLoader(application.data());
    auto bookDatabaseManager = new BookDatabaseManager(application.data());


    //view->rootContext()->setContextProperty("bookLoader", bookLoader);
    view->rootContext()->setContextProperty("bookFinder", bookFinder);
    view->rootContext()->setContextProperty("bookDatabaseManager", bookDatabaseManager);

    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/NovelNest.qml")));
    view->show();

    return application->exec();
}
