import QtQuick 2.7

Rectangle {
    id: editBox
    height: 400; width: 370
    color: "#8667c2"

    TextEdit {
        id: cardEdit
        color: "#dddddd"
        text: "hello" //cardText
        focus: true
        font.pointSize: 12
        wrapMode: Text.Wrap
        leftPadding: 10
        topPadding: 8
        anchors.fill: parent
        visible: true
        onEditingFinished: { editedData = text }
    }
}
