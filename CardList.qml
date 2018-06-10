import QtQuick 2.0
import QtQuick.Layouts 1.1

ListView {
    Layout.fillWidth: true
    Layout.minimumWidth: 300
    Layout.preferredWidth: 400
    Layout.maximumWidth: 800
    Layout.minimumHeight: 450
    Layout.preferredHeight: 800
    delegate: CardView {    }
}
