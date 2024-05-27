import QtQuick 2.0
import Sailfish.Silica 1.0
import BookLoader 1.0

Page {
    id: readingPage
    allowedOrientations: Orientation.All
    property var book

    BookLoader {
        id: bookLoader
    }

    PageHeader {
        id: titleHeader
        title: book ? book.title : qsTr("Reading")
    }


    SilicaFlickable {
        objectName: "flickable"
        anchors.fill: parent
        contentHeight: column.height

        onContentYChanged: {
            titleHeader.visible = contentY < 300
        }

        Column {
            id: column
            width: page.width
            spacing: Theme.paddingLarge

            Text{
                id: bookContent

                text: bookLoader.loadBookContent(book.filePath);
                color: "white"
                wrapMode: Text.WordWrap
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                font.pixelSize: Theme.fontSizeMedium // Динамически изменяем размер шрифта

            }
        }
    }
}

