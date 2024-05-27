import QtQuick 2.0
import Sailfish.Silica 1.0



Page {
    id: searchPage
    objectName: "searchPage"
    allowedOrientations: Orientation.All

    property var booksModel: []

    Component.onCompleted: {
        var existingBooks = bookDatabaseManager.loadBooks();
        bookFinder.findFb2Books(existingBooks);
    }

    Connections {
        target: bookFinder
        onBooksFound: {
            booksModel = arguments[0];
            bookListView.model = booksModel; //Исправить
        }
    }

    Column {
        width: parent.width
        spacing: Theme.paddingMedium

        PageHeader {
            objectName: "pageHeader"
            title: qsTr("Found Books")
        }

        ListView {
            id: bookListView
            width: parent.width
            height: parent.height

            delegate: Item {
                width: parent.width
                height: 80

                Column {
                    width: parent.width
                    spacing: 5

                    Text {
                        text: modelData.split("::")[0]
                        color: "white"
                        font.pixelSize: 40
                    }
                    Text {
                        text: modelData.split("::")[1]
                        color: "white"
                        font.pixelSize: 20
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Clicked on item with title: " + modelData.split("::")[0])

                        var filePath = modelData.split("::")[1]
                        var bookInfo = bookFinder.extractBookInfo(filePath)
                        bookDatabaseManager.addBook(bookInfo.title, bookInfo.author, filePath);
                        //bookDatabaseManager.addBook("Имя", "Автор", filePath);

                        booksModel.splice(index, 1);
                        bookListView.model = booksModel;
                    }
                }
            }

        }

    }
}
