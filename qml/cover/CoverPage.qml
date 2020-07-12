import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {

    Column {
        anchors.centerIn: parent
        spacing: Theme.paddingLarge * 2

        Image {
            source: "/usr/share/icons/hicolor/172x172/apps/nanofiles.png"
            width: 2/3 * parent.width
            fillMode: Image.PreserveAspectFit
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Label {
            id: label
            text: qsTr("Nanofiles")
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
