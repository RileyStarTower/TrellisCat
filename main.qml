import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.1

ApplicationWindow {
    id: appWindow
    visible: true
    visibility: Window.Maximized
    width: 640
    height: 480
    title: qsTr("TrellCat")
    color: "#160732"

    signal addChild(int index)

    menuBar: MenuBar {
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
        }
    }

    GridView {
        id: cardGrid
        objectName: "cardGrid"
        model: gridModel
        x: 10; y: 10
        width: 400
        height: parent.height
        cellHeight: 110
        cellWidth: 400
        delegate: CardView {}
//        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        Keys.onPressed: {
            if ((event.key === Qt.Key_Right) && (event.modifiers & Qt.ControlModifier)) {
                addChild(currentIndex)
            }
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
        // , *.🐱
        nameFilters: [ "TrellCat files (*.tcpro *.🐱)", "All files (*)" ]
        selectedNameFilter: nameFilters[0]
        onAccepted: {
            console.log("Accepted: " + fileUrls)
            if (fileDialogOpenFiles.checked)
                for (var i = 0; i < fileUrls.length; ++i)
                    Qt.openUrlExternally(fileUrls[i])
        }
        onRejected: { console.log("Rejected: " + buildExtensions()) }

        function buildExtensions () {
            return "*.tcpro, *.🐱"
        }
    }
}
