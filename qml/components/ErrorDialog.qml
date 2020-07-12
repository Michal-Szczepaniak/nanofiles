import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    id: errorDialog
    property string error
    canAccept: false
    allowedOrientations: Orientation.All

    Column {
        width: parent.width

        DialogHeader {
            title: qsTr("Error")
            cancelText: qsTr("Okay")
        }

        Label {
            id: nameField
            text: errorDialog.error
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }
    }
}
