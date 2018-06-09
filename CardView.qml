import QtQuick 2.7
import QtQuick.Layouts 1.1

RowLayout {

Rectangle {
    id: leftBuffer
    width: 20; height: 100
    color: "#160732"
}

Rectangle {
    id: cardBox
    width: 370; height: 100

    TextEdit {
        id: cardEdit
        color: "#dddddd"
        text: cardText
        font.pointSize: 12
        wrapMode: Text.Wrap
        leftPadding: 10
        topPadding: 15
        anchors.fill: parent
        visible: true
    }

    Rectangle {
        id: divider
        width: 400
        height: 10
        color: "#160732"
    }

    states: [
        State {
            name: "card"
            when: cardType == 1
            PropertyChanges {
                target: cardEdit
                visible: true
            }
            PropertyChanges {
                target: cardBox
                color: "#6647a2"
            }
        },

        State {
            // spacers fill out the lists so the tree looks like a tree
            name: "spacer"
            when: cardType == 2
            PropertyChanges {
                target: cardEdit
                visible: false
            }
            PropertyChanges {
                target: cardBox
                color: "#160732"
            }
        }

    ]
}
}
