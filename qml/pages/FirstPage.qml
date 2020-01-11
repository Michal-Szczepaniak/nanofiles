import QtQuick 2.5
import Sailfish.Silica 1.0
import QtMultimedia 5.6
import QtQml.Models 2.2
import Qt.labs.folderlistmodel 2.1

Page {
    id: page

    property bool landscape: ( page.orientation === Orientation.Landscape || page.orientation === Orientation.LandscapeInverted )

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Show Page 2")
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: parent.height

        FolderListModel {
            id: flm
            showDotAndDotDot: true
        }

        SilicaListView {
            id: filesListView
            anchors.fill: parent
            model: flm
            visible: false
            delegate: BackgroundItem {
                Label {
                    text: fileName
                }

                onClicked: {
                    flm.folder = filePath
                }
            }
        }

        SilicaGridView {
            id: filesGridView
            anchors.fill: parent
            model: flm
            visible: true
            cellWidth: landscape ? Screen.height / Math.round(Screen.height / (Screen.width/3)) :  Screen.width/3
            cellHeight: cellWidth

            delegate: BackgroundItem {
                width: filesGridView.cellWidth
                height: width
                clip: true

                Icon {
                    id: icon
                    width: parent.width
                    height: parent.height/2
                    source: fileIsDir ? "image://theme/icon-m-file-folder" : "image://theme/icon-m-file-document-light"
                    fillMode: Image.PreserveAspectFit
                }

                Label {
                    text: fileName
                    anchors.top: icon.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: Theme.paddingSmall
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    font.pixelSize: Theme.fontSizeSmall
                    truncationMode: TruncationMode.Elide
                }

                onClicked: {
                    flm.folder = filePath
                }
            }
        }
    }
}
