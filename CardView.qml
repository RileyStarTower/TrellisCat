import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

FocusScope {
//    property alias color: rectangle.color
    x: cardStuff.x; y: cardStuff.y
    width: cardStuff.width; height: cardStuff.height
    RowLayout {
        id: cardStuff
        width: 400; height: 105

        // The horizontal connector from a child Card to the parent list
        Rectangle {
            id: horizontalConnector
            width: 15; height: 5
            anchors.right: cardBox.left
            color: "#160732"

            states: [
                State {
                    // spacer cards don't have horizontal connectors
                    when: cardType == 2
                    PropertyChanges {
                        target: horizontalConnector
                        color: "#160732"
                        // we change the color instead of the visibility because we still need it to take up space
                    }
                },
                State {
                    // first child, extend to parent
                    when: childType == 1
                    PropertyChanges {
                        target: horizontalConnector
                        color: "#dddddd"
                        width: 30;
                    }
                },
                State {
                    // middle child, extend halfway to parent
                    when: childType == 2
                    PropertyChanges {
                        target: horizontalConnector
                        color: "#dddddd"
                        width: 15;
                    }
                }
            ]
        }

        // The vertical connector from one sibling to other siblings
        Rectangle {
            id: verticalConnector
            width: 5; height: 100
            anchors.horizontalCenter: horizontalConnector.horizontalCenter
            color: "#160732"

            states: [
                State {
                    // only child, and the left-most cards, no vertical connectors
                    when: siblingType == 0
                    PropertyChanges {
                        target: verticalConnector
                        color: "#000000ff"
                    }
                },
                State {
                    // first child, extend from middle down to sibling
                    when: siblingType == 1
                    PropertyChanges {
                        target: verticalConnector
                        color: "#dddddd"
                        anchors.horizontalCenter: horizontalConnector.horizontalCenter
                        anchors.top: horizontalConnector.verticalCenter
                        height: 65
                    }
                },
                State {
                    // middle child, extend between siblings
                    when: siblingType == 2 || cardType == 2
                    PropertyChanges {
                        target: verticalConnector
                        color: "#dddddd"
                        height: 105
                        anchors.horizontalCenter: horizontalConnector.left
                    }
                },
                State {
                    // last child, extend from middle up to sibling
                    when: siblingType == 3
                    PropertyChanges {
                        target: verticalConnector
                        color: "#dddddd"
                        anchors.horizontalCenter: horizontalConnector.left
                        anchors.bottom: horizontalConnector.bottom
                        height: 65
                    }
                }
            ]
        }

        Rectangle {
            id: cardBox
            width: 370; height: 100
            anchors.left: parent.left
            color: "#6647a2"
            clip: true

            TextInput {
                id: cardEdit
                color: "#dddddd"
                text: cardText
                focus: true
                font.pointSize: 12
                wrapMode: Text.Wrap
                leftPadding: 10
                topPadding: 8
                anchors.fill: parent
                visible: true
                onEditingFinished: { editedData = text }
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
                    PropertyChanges {
                        target: shadow
                        visible: true
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
                    PropertyChanges {
                        target: shadow
                        visible: false
                    }
                }
            ]
        }

        DropShadow {
            id: shadow
            anchors.fill: cardBox
            horizontalOffset: 3
            verticalOffset: 5
            radius: 10
            samples: 17
            color: "#462782"
            source: cardBox
        }
    }
}
