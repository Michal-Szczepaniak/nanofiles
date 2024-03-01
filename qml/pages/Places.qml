import QtQuick 2.2
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0

Page {

    property var flm

    allowedOrientations: Orientation.All

    property string defaultPlaces: JSON.stringify([
                                                      {name: "Home", path: StandardPaths.home},
                                                      {name: "Documents", path: StandardPaths.documents},
                                                      {name: "Downloads", path: StandardPaths.download},
                                                      {name: "Music", path: StandardPaths.music},
                                                      {name: "Pictures", path: StandardPaths.pictures},
                                                      {name: "Videos", path: StandardPaths.videos},
                                                      {name: "Android storage", path: StandardPaths.home + "/android_storage"},
                                                      {name: "Root", path: "/"},
                                                      {name: "SD card", path: engine.getSdCardMountPath()},
                                                  ])

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: parent.height

        PageHeader {
            id: pageHeader
            title: "Places"
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("About Nanofiles")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }
            MenuItem {
                text: qsTr("Reset entries")
                onClicked: settings.places = defaultPlaces
            }
            MenuItem {
                text: engine.rootMode ? qsTr("Restart in user mode") : qsTr("Restart in root mode")
                onClicked: {
                    engine.rootMode = !engine.rootMode
                }
            }
        }

        ConfigurationGroup {
            id: settings
            path: "/apps/nanofiles"

            property string places: defaultPlaces

            onPlacesChanged: {
                listModel.clear()
                JSON.parse(settings.places).forEach(function(element){ listModel.append(element) })
            }
        }

        SilicaListView {
            id: places
            anchors.top: pageHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            model: ListModel {
                id: listModel
            }
            delegate: ListItem {
                contentHeight: Theme.itemSizeLarge
                Row {
                    id: row
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: Theme.paddingLarge
                    spacing: Theme.paddingLarge

                    Icon {
                        source: "image://theme/icon-m-file-folder"
                    }

                    Label {
                        height: parent.height
                        text: name
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("Move Up")
                        enabled: index > 0
                        onClicked: {
                            var tmpPlaces = JSON.parse(settings.places)
                            tmpPlaces.splice(index-1, 0, tmpPlaces.splice(index, 1)[0])
                            settings.places = JSON.stringify(tmpPlaces)
                        }
                    }

                    MenuItem {
                        text: qsTr("Move Down")
                        enabled: index < listModel.count - 1
                        onClicked: {
                            var tmpPlaces = JSON.parse(settings.places)
                            tmpPlaces.splice(index+1, 0, tmpPlaces.splice(index, 1)[0])
                            settings.places = JSON.stringify(tmpPlaces)
                        }
                    }

                    MenuItem {
                        text: qsTr("Rename")
                        onClicked: pageStack.push(nameChangeDialog, {name: name, id: index})
                    }

                    MenuItem {
                        text: qsTr("Delete")
                        onClicked: {
                            var tmpPlaces = JSON.parse(settings.places)
                            tmpPlaces.splice(index, 1)
                            settings.places = JSON.stringify(tmpPlaces)
                        }
                    }
                }

                onClicked: {
                    flm.folder = path
                    pageStack.navigateBack()
                }
            }
        }
    }

    Dialog {
        id: nameChangeDialog

        property string name
        property int id

        allowedOrientations: Orientation.All

        Column {
            width: parent.width

            DialogHeader {
                title: qsTr("Change name")
            }

            TextField {
                id: nameField
                width: parent.width
                placeholderText: nameChangeDialog.name
                label: "Name"
            }
        }

        onDone: {
            if (result == DialogResult.Accepted) {
                var tmpPlaces = JSON.parse(settings.places)
                tmpPlaces[id].name = nameField.text
                settings.places = JSON.stringify(tmpPlaces)
            }
        }
    }
}
