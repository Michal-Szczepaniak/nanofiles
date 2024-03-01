import QtQuick 2.5
import Sailfish.Silica 1.0
import "../components"

Page {
    id: developerspage
        allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: content.height

        VerticalScrollDecorator {}

        PullDownMenu {
            MenuItem {
                text: qsTr("POEditor translations")
                onClicked: Qt.openUrlExternally("https://poeditor.com/join/project/E3P8NfVEMc")
            }
        }

        Column {
            id: content
            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                id: pageheader
                title: qsTr("Nanofiles translators")
            }

            CollaboratorsLabel {
                title: "Spanish"
                labelData: [ "Carmenfdezb" ]
            }
            CollaboratorsLabel {
                title: "Hungarian"
                labelData: [ "1Zgp" ]
            }
            CollaboratorsLabel {
                title: "Chinese"
                labelData: [ "dashinfantry" ]
            }
            CollaboratorsLabel {
                title: "Swedish"
                labelData: [ "eson57" ]
            }
        }
    }
}
