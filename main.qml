import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.1
import com.startowerstudio.cardmodel 1.0
import "loadCardGrid.js" as TCFileManager

ApplicationWindow {
    id: appWindow
    visible: true
    visibility: Window.Maximized
    width: 640
    height: 480
    title: qsTr("TrellCat")
    color: "#1607a2"

    signal addChild(int index)

//    property int gridWidth: 1200
//    CardModel {
//        id: currentModel
//    }

    menuBar: MenuBar {
        id: menuBar
        Menu {
            title: "&File"
            MenuItem {
                text: "E&xit"
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
            MenuItem {
                text: "&Open"
                shortcut: StandardKey.Open
                onTriggered: fileDialog.open()
            }
            MenuItem {
                text: "&Close"
                shortcut: StandardKey.Close
                onTriggered: closeProject()
            }
        }
    }

    TabView {
        anchors.fill: parent
        id: tabView
        height: parent.height

        style: TabViewStyle {
                frameOverlap: 0
//                tab: Rectangle {
//                    color: styleData.selected ? "lightsteelblue" :"steelblue"
//                    border.color:  "steelblue"
//                    implicitWidth: Math.max(text.width + 20, 80)
//                    implicitHeight: styleData.selected ? 40 : 30
//                    anchors.bottom: parent.bottom
//                    radius: 2
//                    Text {
//                        id: text
//                        anchors.centerIn: parent
//                        font.pointSize: 12
//                        text: styleData.title
//                        color: styleData.selected ? "black" : "white"
//                    }
//                }
                frame: Rectangle { color: "#160732" }
            }
    }

    FileDialog {
        id: fileDialog
        modality: Qt.WindowModal
        title: "Open"
        folder: shortcuts.documents + "/TrellCat Documents/"
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        nameFilters: [ "TrellCat files (*.tcpro *.🐱)", "All files (*)" ]
        selectedNameFilter: nameFilters[0]
        onAccepted: {
            TCFileManager.attemptLoad(fileUrls);
        }
        onRejected: { console.log("Rejected file") }
    }

    function closeProject() {
        var tab = tabView.getTab(tabView.currentIndex);
        tab.item.close();
        tabView.removeTab(tabView.currentIndex);
    }
}
