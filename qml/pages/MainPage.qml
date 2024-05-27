import QtQuick 2.0
import Sailfish.Silica 1.0
import BooksModel 1.0

Page {
    id: mainPage
    objectName: "mainPage"
    allowedOrientations: Orientation.All

    ListModel {
        id: bookListModel
    }

    Component.onCompleted: {
        var books = bookDatabaseManager.loadBooks()
        for (var i = 0; i < books.length; i++) {
            bookListModel.append(books[i])
        }
    }

    Connections {
        target: bookDatabaseManager
        onBookAdded: {
            gridView.model.append(book)
        }
        onBookRemoved: {
            for (var i = 0; i < gridView.model.count; i++) {
                if (bookListModel.get(i).id === bookId) {
                    bookListModel.remove(i)
                    break
                }
            }
        }
    }

    SilicaGridView {
        id: gridView
        anchors {
            leftMargin: 40
            bottomMargin: 100
            fill: parent
        }
        header: PageHeader { title: qsTr("My Books")}
        cellWidth: width / 2
        cellHeight: cellWidth
        delegate: Element {
            id: gridDelegate
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("BookDetailPage.qml"), { "book": model  })
                }
            }

            Label {
                anchors.top: gridDelegate.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                truncationMode: TruncationMode.Fade
                elide: Text.ElideRight
                text: model.title // Отображаем название книги
            }
        }
        model: bookListModel

        VerticalScrollDecorator { }
    }
    Button {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: Theme.buttonWidthExtraSmall
        height: width/2
        color: "white"
        text: "Add Book"
        backgroundColor : "blue"
        onClicked: {
            pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
        }
    }
}


