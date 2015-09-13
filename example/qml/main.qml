import QtQuick.Controls 1.1
import QtQuick 2.2

StackView {
    width: 100
    height: 100

    id: stack
    initialItem: view

    Component {
        id: view

        MouseArea {
            objectName: "view1"
            Text {
                text: "View 1" 
                anchors.centerIn: parent
            }
            onClicked: stack.push(view2)
        }
    }
   Component {
        id: view2
        MouseArea {
            Text {
                text: "View 2" 
                anchors.centerIn: parent
            }
            onClicked: stack.push(view)
        }
    }
}
