import QtQuick 2.2
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0
import "../js/moment.min.js" as Moment

Page {
    id: page

    property var flm
    property var index
    property var icon

    allowedOrientations: Orientation.All

    function get(name) {
        return flm.get(index, name)
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: fileIcon.height + propertiesColumn.height + Theme.paddingLarge * 2

        PageHeader {
            id: pageHeader
            title: "File information"
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Share")
                onClicked: pageStack.push(Qt.resolvedUrl("ShareFilesPage.qml"))
            }
        }

        Image {
            id: fileIcon
            source: icon
            anchors.top: pageHeader.bottom
            anchors.topMargin: Theme.paddingLarge
            width: Theme.itemSizeHuge
            height: Theme.itemSizeHuge
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Column {
            id: propertiesColumn
            anchors.top: fileIcon.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge
            anchors.topMargin: Theme.paddingLarge * 2

            Label {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeExtraLarge
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: get("fileName")
            }

            Row {
                width: parent.width
                height: Theme.itemSizeLarge
                spacing: Theme.paddingLarge

                Label {
                    height: parent.height
                    width: parent.width/2 - Theme.paddingLarge
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignBottom
                    font.pixelSize: Theme.fontSizeSmall
                    text: qsTr("Size:")
                    color: Theme.highlightColor
                }

                Label {
                    height: parent.height
                    width: parent.width/2 - Theme.paddingLarge
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignBottom
                    font.pixelSize: Theme.fontSizeSmall
                    text: get("fileSize")
                }
            }

            Row {
                width: parent.width
                spacing: Theme.paddingLarge

                Label {
                    width: parent.width/2 - Theme.paddingLarge
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: Theme.fontSizeSmall
                    text: qsTr("Modified:")
                    color: Theme.highlightColor
                }

                Label {
                    width: parent.width/2 - Theme.paddingLarge
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: Theme.fontSizeSmall
                    text: Moment.moment(get("fileModified")).format("Y/MM/DD HH:MM:SS")
                }
            }

            Row {
                width: parent.width
                spacing: Theme.paddingLarge

                Label {
                    width: parent.width/2 - Theme.paddingLarge
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: Theme.fontSizeSmall
                    text: qsTr("Accessed:")
                    color: Theme.highlightColor
                }

                Label {
                    width: parent.width/2 - Theme.paddingLarge
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: Theme.fontSizeSmall
                    text: Moment.moment(get("fileAccessed")).format("Y/MM/DD HH:MM:ss")
                }
            }
        }
    }
}
