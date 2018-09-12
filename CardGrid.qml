import QtQuick 2.0
import com.startowerstudio.cardmodel 1.0

GridView {
    id: cardGrid
    CardModel {
        id: currentModel
    }
    model: currentModel

    x: 10; y: 10
    height: parent.height - 10
    cellHeight: 110
    cellWidth: 400
    delegate: CardView {}
//        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
    Keys.onPressed: {
        if ((event.key === Qt.Key_Right) && (event.modifiers & Qt.ControlModifier)) {
            addChild(currentIndex)
        }
    }

    function populateModel(filename, tabView) {
        currentModel.loadNewFile(filename)
        cardGrid.width = Qt.binding(function() { return currentModel.gridWidth })
    }

    function close() {
        currentModel.writeAllCards()
    }
}
