import QtQuick 2.0

Rectangle {
    width: 360
    height: 360
    Text {
        id: helloText
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        onClicked: {
            helloText.text = "CLICKED"
        }
    }
}
