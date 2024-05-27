import QtQuick 2.0

Rectangle {
    width: gridView.width / 2 - 40
    height: gridView.height / 4

    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "blue" }
    }
}
