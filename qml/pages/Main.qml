import QtQuick 2.5
import Sailfish.Silica 1.0
import QtMultimedia 5.6
import QtQml.Models 2.2
import Qt.labs.folderlistmodel 2.1
import SortFilterProxyModel 0.2
import org.nemomobile.configuration 1.0
import "../components"
import "../js/moment.min.js" as Moment

Page {
    id: page

    property bool landscape: ( page.orientation === Orientation.Landscape || page.orientation === Orientation.LandscapeInverted )
    property bool selectMode: false

    function getFileIconByMimeType(filePath, fileSize) {
        var mimeType = fmanager.mimeType(filePath);
        var type = mimeType.split("/")[0]
        var icon = "";

        switch(type) {
        case "audio":
            icon = "image://theme/icon-m-file-audio";
            break;
        case "image":
            if (Math.round(fileSize/1024) < 1024)
                icon = filePath
            else
                icon = "image://theme/icon-m-file-image";
            break;
        case "video":
            icon = "image://theme/icon-m-file-video";
            break;
        default:
            icon = "image://theme/icon-m-file-document-dark";
        }

        switch(mimeType) {
        case "application/pdf":
            icon = "image://theme/icon-m-file-pdf-light";
            break;
        case "application/vnd.ms-excel":
        case "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet":
        case "application/vnd.oasis.opendocument.spreadsheet":
            icon = "image://theme/icon-m-file-spreadsheet-light"
            break;
        case "application/vnd.ms-powerpoint":
        case "application/vnd.openxmlformats-officedocument.presentationml.presentation":
        case "application/vnd.oasis.opendocument.presentation":
            icon = "image://theme/icon-m-file-presentation-light"
            break;
        case "application/msword":
        case "application/vnd.openxmlformats-officedocument.wordprocessingml.document":
        case "application/vnd.oasis.opendocument.text":
            icon = "image://theme/icon-m-file-formatted-light"
            break;
        case "application/x-rar-compressed":
        case "application/zip":
        case "application/x-tar":
        case "application/x-bzip":
        case "application/x-bzip2":
        case "application/gzip":
        case "application/x-7z-compressed":
        case "application/x-lzma":
        case "application/x-xz":
            icon = "image://theme/icon-m-file-archive-folder"
            break;
        case "application/vnd.android.package-archive":
            icon = "image://theme/icon-m-file-apk"
            break;
        case "application/x-rpm":
            icon = "image://theme/icon-m-file-rpm"
            break;
        }

        return icon;
    }

    function handleButton(func) {
        switch(func) {
        case "select":
            clipboard.changeFileOperation("", getCurrentDir())
            clipboard.clearClipboard()
            clipboard.clearSelectedFiles()
            selectMode = false
            break;
        case "copy":
            if (clipboard.getClipboardFileCount()) {
                engine.performFileOperation("paste",
                                            clipboard.getClipboard(),
                                            clipboard.getClipboardDir(),
                                            clipboard.getSelectedFiles(),
                                            getCurrentDir())
                clipboard.changeFileOperation("", getCurrentDir())
                clipboard.clearClipboard()
            } else {
                clipboard.changeFileOperation("copy", getCurrentDir())
                clipboard.clearSelectedFiles()
                selectMode = false
            }
            break;
        case "delete":
            Remorse.popupAction(page, Remorse.deletedText, function() {
                if (clipboard.getSelectedFileCount()) {
                    engine.performFileOperation("delete",
                                                clipboard.getClipboard(),
                                                clipboard.getClipboardDir(),
                                                clipboard.getSelectedFiles(),
                                                getCurrentDir())
                    clipboard.changeFileOperation("", getCurrentDir())
                    selectMode = false
                } else {
                    clipboard.changeFileOperation("", getCurrentDir())
                    selectMode = false
                }
                clipboard.clearClipboard()
                clipboard.clearSelectedFiles()
            })
            break;
        case "cut":
            if (clipboard.getClipboardFileCount()) {
                engine.performFileOperation("paste",
                                            clipboard.getClipboard(),
                                            clipboard.getClipboardDir(),
                                            clipboard.getSelectedFiles(),
                                            getCurrentDir())
                clipboard.changeFileOperation("", getCurrentDir())
                clipboard.clearClipboard()
            } else {
                clipboard.changeFileOperation("cut", getCurrentDir())
                clipboard.clearSelectedFiles()
                selectMode = false
            }
            break;
        case "tar":
            if (clipboard.getSelectedFileCount()) {
                var tarFile = getCurrentDir() + "/nanofiles-" + Date.now() + ".tar.gz";
                engine.tarFiles(tarFile, getCurrentDir(), clipboard.getSelectedFiles());
            } else {
                clipboard.changeFileOperation("", getCurrentDir())
            }
            selectMode = false
            clipboard.clearSelectedFiles()
            break;
        case "rename":
            pageStack.push(Qt.resolvedUrl("../components/FileRenameDialog.qml"), { "files": clipboard.getSelectedFiles() })
            selectMode = false
            clipboard.clearSelectedFiles()
            break;
        case "info":
            var fileIndex = flm.indexOf("file:" + clipboard.getSelectedFiles()[0])
            pageStack.push(Qt.resolvedUrl("FileInfo.qml"), { flm: flm, index: fileIndex, icon: (flm.get(fileIndex, "fileIsDir") ? "image://theme/icon-m-file-folder" : getFileIconByMimeType(clipboard.getSelectedFiles()[0], 0))})
            break;
        }
    }

    function getCurrentDir() {
        return flm.folder.toString().replace("file://", "")
    }

    onStatusChanged: {
        if (status === PageStatus.Active && pageStack.depth === 1) {
            pageStack.pushAttached(Qt.resolvedUrl("Places.qml"), {flm: flm})
        }
    }

    ConfigurationGroup {
        id: settings
        path: "/apps/nanofiles"

        property string places: JSON.stringify([
                                                   {name: "Home", path: StandardPaths.home},
                                                   {name: "Documents", path: StandardPaths.documents},
                                                   {name: "Downloads", path: StandardPaths.download},
                                                   {name: "Music", path: StandardPaths.music},
                                                   {name: "Pictures", path: StandardPaths.pictures},
                                                   {name: "Videos", path: StandardPaths.videos},
                                                   {name: "Android storage", path: StandardPaths.home + "/android_storage"},
                                                   {name: "Root", path: "/"},
                                                   {name: "SD card", path: "/media/sdcard/"},
                                               ])
        property bool listView: false
    }

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }

            MenuItem {
                text: engine.rootMode ? qsTr("Restart in user mode") : qsTr("Restart in root mode")
                onClicked: {
                    engine.rootMode = !engine.rootMode
                    Qt.quit()
                }
            }

            MenuItem {
                text: settings.listView ? qsTr("Switch to grid view") : qsTr("Switch to list view")
                onClicked: settings.listView = !settings.listView
            }

            MenuItem {
                text: qsTr("Add to places")
                onClicked: {
                    var tmpPlaces = JSON.parse(settings.places)
                    tmpPlaces.push({name: pageHeader.title, path: getCurrentDir()})
                    settings.places = JSON.stringify(tmpPlaces)
                }
            }

            MenuItem {
                text: flm.showHiddenFiles ? qsTr("Hide hidden files") : qsTr("Show hidden files")
                onClicked: {
                    flm.showHiddenFiles = !flm.showHiddenFiles
                }
            }

            MenuItem {
                text: qsTr("New…")
                onClicked: pageStack.push(Qt.resolvedUrl("../components/NewFilesDialog.qml"), {path: getCurrentDir()})
            }
        }

        PageHeader {
            id: pageHeader
            width: parent.width
            title: {
                var path = flm.folder.toString().split("/")
                if (path[path.length - 1] !== "")
                    return path[path.length - 1]
                else
                    return path[path.length - 2]
            }
        }

        contentHeight: parent.height

        FolderListModel {
            id: flm
            folder: StandardPaths.home
            showDotAndDotDot: true
            showDirsFirst: true
            showHidden: true

            property bool showHiddenFiles: false
        }

        SortFilterProxyModel {
            id: flmProxyModel
            sourceModel: flm

            filters: [
                ValueFilter {
                    roleName: "fileName"
                    value: "."
                    inverted: true
                    enabled: true
                },
                RegExpFilter {
                    roleName: "fileName"
                    pattern: "^\\..{2,}"
                    inverted: true
                    enabled: !flm.showHiddenFiles
                }
            ]

            sorters: [
                ExpressionSorter {
                    expression: {
                        return modelLeft.fileName === "..";
                    }
                }
            ]
        }

        SilicaListView {
            id: lv
            anchors.top: pageHeader.bottom
            anchors.bottom: menuBar.top
            anchors.left: parent.left
            anchors.right: parent.right
            orientation: ListView.Horizontal
            layoutDirection: ListView.RightToLeft
            interactive: true
            clip: true
            contentHeight: height
            contentWidth: width*2
            snapMode: ListView.SnapOneItem

            onMovingChanged: {
                if (!moving) {
                    currentIndex = (contentX + page.width*2) === 0 ? 0 : 1 //idk why it's shifted by 2*page.width
                }
            }

            onCurrentIndexChanged: {
                if (currentIndex == 1) {
                    flm.folder = flm.get(1, "filePath")
                    lv.currentIndex = 0
                }
            }

            model: ListModel {
                id: listModel

                ListElement {}
                ListElement {}
            }


            delegate: Item {
                width: lv.width
                height: lv.height

                Loader {
                    id: filesViewLoader
                    active: true
                    anchors.fill: parent
                    sourceComponent: settings.listView ? listComponent : gridComponent
                }

                Component {
                    id: listComponent
                    SilicaListView {
                        id: filesListView
                        anchors.fill: parent
                        model: flmProxyModel
                        clip: true
                        visible: settings.listView
                        cacheBuffer: BackgroundItem.height*10

                        delegate: BackgroundItem {
                            down: listItemMouseArea.pressed
                            property bool selected: contains(clipboard.getSelectedFiles(), filePath)

                            function contains(array, string) {
                                var result = false
                                array.forEach(function(element) { if(element === string) result = true })
                                return result
                            }

                            Rectangle {
                                anchors.fill: parent
                                color: Theme.rgba(Theme.secondaryHighlightColor, Theme.highlightBackgroundOpacity)
                                visible: selected
                            }

                            Row {
                                anchors.left: parent.left
                                anchors.leftMargin: Theme.paddingLarge
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right
                                anchors.rightMargin: Theme.paddingLarge
                                spacing: Theme.paddingLarge

                                Image {
                                    id: listIcon
                                    width: parent.height
                                    height: parent.height
                                    source: fileIsDir ? "image://theme/icon-m-file-folder" : getFileIconByMimeType(filePath, fileSize)
                                    fillMode: Image.PreserveAspectFit
                                    asynchronous: true
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Column {
                                    width: parent.width - Theme.paddingLarge - listIcon.width
                                    anchors.verticalCenter: parent.verticalCenter
                                    Label {
                                        width: parent.width - Theme.paddingLarge - listIcon.width
                                        clip: true
                                        truncationMode: TruncationMode.Elide
                                        text: fileName
                                    }
                                    Label {
                                        width: parent.width - Theme.paddingLarge - listIcon.width
                                        clip: true
                                        truncationMode: TruncationMode.Elide
                                        font.pixelSize: Theme.fontSizeExtraSmall
                                        color: Theme.secondaryColor
                                        text: qsTr("Size: %1 Modified: %2").arg(fileSize.toString()).arg(Moment.moment(fileModified).format("Y/MM/DD HH:MM"))
                                    }
                                }
                            }

                            MouseArea {
                                id: listItemMouseArea

                                anchors.fill: parent
                                propagateComposedEvents: true

                                function selectFile(index, filePath) {
                                    if (!selected) {
                                        clipboard.addFileToSelectedFiles(filePath)
                                        if(!selectMode) selectMode = true
            //                            selectedFiles[index] = filePath
                                    } else {
                                        clipboard.removeFileFromSelectedFiles(filePath)
            //                            delete selectedFiles[index];
                                    }

                                    selected = !selected
                                    if (clipboard.selectedFileCount === 0) selectMode = false
                                }

                                onPressAndHold: {
                                    selectFile(index, filePath)
                                }

                                onClicked: {
                                    if (selectMode) {
                                        selectFile(index, filePath)
                                    } else if (fileIsDir) {
                                        flm.folder = filePath
                                    } else {
                                        fprocess.performFileAction(filePath, "openSystem", false)
                                    }
                                }
                            }
                        }
                    }
                }

                Component {
                    id: gridComponent
                    SilicaGridView {
                        id: filesGridView
                        anchors.fill: parent
                        model: flmProxyModel
                        cellWidth: landscape ? Screen.height / Math.round(Screen.height / (Screen.width/3)) :  Screen.width/3
                        cellHeight: cellWidth + Theme.paddingSmall
                        clip: true
                        visible: !settings.listView
                        cacheBuffer: (filesGridView.cellWidth + width + Theme.paddingSmall)*10

                        delegate: BackgroundItem {
                            width: filesGridView.cellWidth
                            height: width + Theme.paddingSmall
                            clip: true
                            down: gridItemMouseArea.pressed

                            property bool selected: contains(clipboard.getSelectedFiles(), filePath)

                            function contains(array, string) {
                                var result = false
                                array.forEach(function(element) { if(element === string) result = true })
                                return result
                            }

                            Rectangle {
                                anchors.fill: parent
                                color: Theme.rgba(Theme.secondaryHighlightColor, Theme.highlightBackgroundOpacity)
                                visible: selected
                            }

                            Image {
                                id: gridicon
                                width: parent.width
                                height: parent.height/2
                                source: fileIsDir ? "image://theme/icon-m-file-folder" : getFileIconByMimeType(filePath, fileSize)
                                fillMode: Image.PreserveAspectFit
                                asynchronous: true
                            }

                            Label {
                                text: fileName
                                anchors.top: gridicon.bottom
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: Theme.paddingLarge
                                anchors.leftMargin: Theme.paddingLarge
                                anchors.rightMargin: Theme.paddingLarge
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                                font.pixelSize: Theme.fontSizeExtraSmall
                                truncationMode: TruncationMode.Elide
                                clip: true
                            }

                            MouseArea {
                                id: gridItemMouseArea

                                anchors.fill: parent
                                propagateComposedEvents: true

                                function selectFile(index, filePath) {
                                    if (!selected) {
                                        clipboard.addFileToSelectedFiles(filePath)
                                        if(!selectMode) selectMode = true
                                    } else {
                                        clipboard.removeFileFromSelectedFiles(filePath)
                                    }

                                    selected = !selected
                                    if (clipboard.selectedFileCount === 0) selectMode = false
                                }

                                onPressAndHold: {
                                    selectFile(index, filePath)
                                }

                                onClicked: {
                                    if (selectMode) {
                                        selectFile(index, filePath)
                                    } else if (fileIsDir) {
                                        flm.folder = filePath
                                    } else {
                                        fprocess.performFileAction(filePath, "openSystem", false)
                                    }
                                }
                            }

                            Connections {
                                target: page
                                onSelectModeChanged: {
                                    if (!selectMode) selected = false
                                }
                            }
                        }
                    }
                }
            }
        }

        MenuBar {
            id: menuBar
            view: page
            height: Theme.itemSizeMedium
            anchors.bottomMargin: hidden ? -height : 0
            iconArray: [ selectMode ? "image://theme/icon-m-close" : "image://theme/icon-m-certificates", "image://theme/icon-m-clipboard", "image://theme/icon-m-delete", "qrc:///images/icon-m-scissors.svg", "image://theme/icon-m-file-archive-folder", "image://theme/icon-m-edit", "image://theme/icon-m-about"]
            functionsArray: [ "select", "copy", "delete", "cut", "tar", "rename", "info" ]

            property bool hidden: !selectMode && !clipboard.clipboardFileCount

            Connections {
                target: clipboard

                onFileOperationChanged: {
                    var tempIconArray = menuBar.iconArray
                    if (clipboard.getFileOperation() === "cut") {
                        if (tempIconArray[3] === "image://theme/icon-m-redirect") {
                            tempIconArray.splice(3,1)
                        }
                        menuBar.iconArray = tempIconArray
                    } else {
                        if (tempIconArray[3] !== "image://theme/icon-m-redirect") {
                            tempIconArray.splice(3, 0, "image://theme/icon-m-redirect")
                        }
                        menuBar.iconArray = tempIconArray
                    }
                }

                onSelectedFileCountChanged: {
                    var tempIconArray = menuBar.iconArray
                    if (clipboard.getSelectedFileCount() > 1) {
                        if (tempIconArray[tempIconArray.length-1] === "image://theme/icon-m-about") {
                            tempIconArray.pop()
                        }
                        menuBar.iconArray = tempIconArray
                    } else {
                        if (tempIconArray[tempIconArray.length-1] !== "image://theme/icon-m-about") {
                            tempIconArray.push("image://theme/icon-m-about")
                        }
                        menuBar.iconArray = tempIconArray
                    }
                }
            }

            Behavior on anchors.bottomMargin {
                PropertyAnimation {
                    duration: 200
                    easing.type: Easing.OutQuint
                }
            }
        }
    }
}
