import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: bookDetailPage
    property var book

    PageHeader {
        id: titleHeader
        title: book ? book.title : qsTr("Book Details")
    }

    Rectangle {
        id: imageRect
        anchors.top: titleHeader.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width / 2
        height: width * 2 / 1.4
        gradient: Gradient {
            GradientStop { position: 0.0; color: "magenta" }
            GradientStop { position: 1.0; color: "white" }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                pageStack.push(Qt.resolvedUrl("ReadingPage.qml"), { "book": book })
            }
        }
    }

    Rectangle {
        id: additionalRect
        anchors.top: imageRect.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: imageRect.width + 20
        height: 80
        color: "green"
        Text {
            anchors.centerIn: parent
            text: book ? qsTr("Author: %1").arg(book.author) : ""
            color: Theme.primaryColor
        }
    }

    Slider {
        id: pageSlider
        anchors.top: additionalRect.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        handleVisible: false
        minimumValue: 0
        maximumValue: 200
        value: 150
        enabled: false
    }

    Label {
        id: shortText
        anchors.top: pageSlider.bottom
        width: parent.width
        padding: Theme.paddingMedium
        text: book ? qsTr("File Path: %1").arg(book.filePath) : qsTr("Something very important")
        truncationMode: TruncationMode.Fade
    }

    Button {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: Theme.buttonWidthExtraSmall
        height: width / 2
        color: "white"
        text: "Delete"
        backgroundColor: "blue"
        onClicked: {
            bookDatabaseManager.delBook(book.id)
            pageStack.pop() // Go back to the previous page after deletion
        }
    }
}
