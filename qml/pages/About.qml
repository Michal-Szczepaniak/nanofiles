/*
    Copyright (C) 2018 Michał Szczepaniak

    This file is part of Morsender.

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

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: aboutPage

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height + header.height + Theme.paddingLarge

        PullDownMenu
        {
            MenuItem
            {
                text: qsTr("Nanofiles Translations")
                onClicked: pageStack.push(Qt.resolvedUrl("TranslationsPage.qml"))
            }
        }

        PageHeader {
            id: header
            title: qsTr("About")
        }

        Column {
            id: column
            spacing: Theme.paddingLarge
            anchors.top: parent.top
            anchors.topMargin: header.height
            width: parent.width

            Image {
                source: "/usr/share/icons/hicolor/172x172/apps/nanofiles.png"
                width: 2/3 * parent.width
                fillMode: Image.PreserveAspectFit
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                text: "Nanofiles 1.4"
                font.pixelSize: Theme.fontSizeExtraLarge
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
            }

            Label {
                text: qsTr("Made by Michał Szczepaniak. Based on fileman.")
                font.pixelSize: Theme.fontSizeSmall
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                width: parent.width - Theme.paddingLarge*2
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                 text: qsTr("Contributors:")
                 font.pixelSize: Theme.fontSizeLarge
                 anchors.horizontalCenter: parent.horizontalCenter
                 wrapMode: Text.WordWrap
                 width: parent.width - Theme.paddingLarge*2
                 horizontalAlignment: Text.AlignHCenter
            }

            Label {
                text: qsTr("Rudi Timmermans (rudi1970)")
                font.pixelSize: Theme.fontSizeSmall
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                width: parent.width - Theme.paddingLarge*2
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                text: qsTr("Licensed under GPLv3")
                font.pixelSize: Theme.fontSizeSmall
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Separator {
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            Label {
                text: qsTr("Links")
                font.pixelSize: Theme.fontSizeLarge
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Github: <a href=\"https://github.com/Michal-Szczepaniak/nanofiles\">github.com/Michal-Szczepaniak/nanofiles</a>"
                color: "white"
                textFormat: Text.StyledText
                font.pixelSize: Theme.fontSizeSmall
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                horizontalAlignment: Text.AlignHCenter
                linkColor: Theme.highlightColor
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Text {
                color: "white"
                textFormat: Text.StyledText
                font.pixelSize: Theme.fontSizeSmall
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                horizontalAlignment: Text.AlignHCenter
                linkColor: Theme.highlightColor
                onLinkActivated: Qt.openUrlExternally(link)
                text: "Twitter: <a href=\"https://twitter.com/Mister1Magister\">twitter.com/Mister1Magister</a>"

            }

            Separator {
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            Label {
                text: qsTr("Donate")
                font.pixelSize: Theme.fontSizeLarge
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
            }

            Text {
                text: "<a href=\"https://www.paypal.me/MisterMagister\">paypal.me/MisterMagister</a>"
                color: "white"
                textFormat: Text.StyledText
                font.pixelSize: Theme.fontSizeSmall
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                horizontalAlignment: Text.AlignHCenter
                linkColor: Theme.highlightColor
                onLinkActivated: Qt.openUrlExternally(link)
            }

       }
       VerticalScrollDecorator {}
    }
}
