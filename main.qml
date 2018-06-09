import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.0

ApplicationWindow {
    id: appWindow
    visible: true
    visibility: Window.Maximized
    width: 640
    height: 480
    title: qsTr("TrellisCat")
    color: "#160732"

    RowLayout {
        CardList {
            id: leftList
            model: cardModel_1
            contentY: middleList.contentY
        }
        CardList {
            id: middleList
            anchors.left: leftList.right
            model: cardModel_2
            contentY: rightList.contentY
        }
        CardList {
            id: rightList
            anchors.left: middleList.right
            model: cardModel_3
            contentY: leftList.contentY
        }
    }
}
