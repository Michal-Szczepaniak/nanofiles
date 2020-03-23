/*
    Copyright (C) 2018 Michał Szczepaniak

    This file is part of Musikilo.

    Morsender is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Morsender is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Morsender.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.1
import Sailfish.Silica 1.0

Item {
    id: tabPageHeader

    property var view: null
    property variant iconArray: []
    property variant functionsArray: []
    property int visibleHeight: flickable.contentY + height

    anchors { bottom: parent.bottom; left: parent.left; right: parent.right }
    height: Theme.itemSizeLarge

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentHeight: parent.height

        Row {
            anchors.fill: parent

            Repeater {
                id: sectionRepeater
                model: iconArray
                delegate: BackgroundItem {
                    width: tabPageHeader.width / sectionRepeater.count
                    height: tabPageHeader.height

                    Image {
                        id: icon
                        height: Theme.iconSizeSmall * 1.4
                        width: Theme.iconSizeSmall * 1.4
                        anchors.centerIn: parent
                        source: modelData
                    }

                   Label {
                        anchors {
                            top: parent.top; topMargin: Theme.paddingSmall
                            left: icon.right; leftMargin: -Theme.paddingMedium
                        }
                        visible: true
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.highlightColor

                        text: ""
                    }

                    Loader {
                        anchors.fill: parent
                        sourceComponent: undefined
                        Component {
                            id: busyIndicator

                            Rectangle {
                                anchors.fill: parent
                                color: "black"
                                opacity: 0

                                Behavior on opacity { NumberAnimation { duration: 250 } }

                                BusyIndicator {
                                    opacity: 1
                                    anchors.centerIn: parent
                                    running: true
                                    height: tabPageHeader.height - Theme.paddingLarge
                                    width: height
                                }

                                Component.onCompleted: opacity = 0.75
                            }
                        }

                    }

                    onClicked: view.handleButton(functionsArray[index])

                }
            }
        }
    }
}
