var cardGridComp;
var filename;

function attemptLoad(fileUrls) {
    cardGridComp = Qt.createComponent("CardGrid.qml");
    filename = fileUrls[0]
    if (cardGridComp.status === Component.Ready) {
        finishLoading();
    } else {
        cardGridComp.statusChanged.connect(finishLoading);
    }
}

function finishLoading() {
    var tab = tabView.addTab("FileName", cardGridComp);
//                tab.item.model = currentModel;
    var cardGridObj = cardGridComp.createObject(tab);
    cardGridObj.populateModel(filename, tabView);
//                currentModel.setProjectFilename(fileUrls[0]);
}
