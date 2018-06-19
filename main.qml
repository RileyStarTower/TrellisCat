import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import QtQuick.Window 2.0

ApplicationWindow {
    id: appWindow
    visible: true
    visibility: Window.Maximized
    width: 640
    height: 480
    title: qsTr("TrellisCat")
    color: "#160732"

    signal addChild(int index)

    GridView {
        id: cardGrid
        model: gridModel
        x: 10; y: 10
        width: 1300
        height: parent.height
        cellHeight: 105
        cellWidth: 400
        delegate: CardView {}
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        Keys.onPressed: {
            if ((event.key === Qt.Key_Right) && (event.modifiers & Qt.ControlModifier))
                addChild(currentIndex)
        }
    }


//    RowLayout {
//        // use small x and y values so the lists are shoved all the way into the corner
//        x: 10; y: 10
//        CardList {
//            id: list_1
//            model: cardModel_1
//            boundsBehavior: Flickable.StopAtBounds
//            ScrollBar.vertical: vBar
//            KeyNavigation.priority: KeyNavigation.BeforeItem
//            KeyNavigation.tab: list_2
//        }
//        CardList {
//            id: list_2
//            anchors.left: list_1.right
//            model: cardModel_2
//            boundsBehavior: Flickable.StopAtBounds
//            ScrollBar.vertical: vBar
//            KeyNavigation.priority: KeyNavigation.BeforeItem
//            KeyNavigation.tab: list_3
//            KeyNavigation.backtab: list_1
//        }
//        CardList {
//            id: list_3
//            anchors.left: list_2.right
//            model: cardModel_3
//            boundsBehavior: Flickable.StopAtBounds
//            ScrollBar.vertical: vBar
//            KeyNavigation.priority: KeyNavigation.BeforeItem
//            KeyNavigation.backtab: list_2
//        }
//    }

//    // Scroll bar that syncs up the scrolling of all the lists
//    ScrollBar {
//        id: vBar
//        height: parent.height
//        anchors.right: parent.right
//    }
}
