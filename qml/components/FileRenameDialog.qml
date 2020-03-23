import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    id: dialog

    property string operation: ""

    property var files: [ ]

    canAccept: true

    allowedOrientations: Orientation.All

    onOpened: {
        updateEntries()
    }

    onAccepted: {
        renameFiles()
    }

    SilicaListView {
        id: listView

        height: parent.height
        width: parent.width

        model: fileModel

        header: DialogHeader {
            id: dialogHeader
            acceptText: qsTr("Rename")
        }
        ListModel {
            id: fileModel
        }

        delegate: Component {
            ListItem {
                contentHeight: column.height

                Column {
                    y: Theme.paddingMedium

                    id: column

                    spacing: Theme.paddingMedium

                    Label {
                        id: fileLabel
                        width: dialog.width - (Theme.paddingLarge * 2)

                        font.pixelSize: Theme.fontSizeSmall

                        color: Theme.secondaryColor

                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                        x: Theme.paddingLarge

                        text: model.sourceName
                    }

                    TextField {
                        id: textField

                        width: dialog.width - Theme.paddingLarge

                        text: model.sourceName

                        onTextChanged: {
                            textField.errorHighlight = !validateFileName(text)
                            updateFileName(model.id, text)
                            validateFileNames()
                        }
                    }
                }
            }
        }
    }

    /*
     *  Add the list of files we are renaming to the model
     */
    function updateEntries()
    {
        for (var i=0; i < files.length; i++)
        {
            var fileName = files[i].substr(files[i].lastIndexOf("/")+1)
            fileModel.append({ "id": i,
                               "fullPath": files[i],
                               "sourceName": fileName,
                               "newName": fileName })
        }
    }

    /*
     *  Rename files
     */
    function renameFiles()
    {
        var files = [];

        for (var i=0; i < fileModel.count; i++)
        {
            var fileEntry = {};
            fileEntry.sourceFullPath = fileModel.get(i).fullPath
            fileEntry.sourceName = fileModel.get(i).sourceName
            fileEntry.newName = fileModel.get(i).newName

            files[files.length] = fileEntry
        }

        // Convert the file array into JSON
        var json = JSON.stringify(files)

        engine.renameFiles(json)
    }

    /*
     *  Validate all file names
     */
    function validateFileNames()
    {
        var invalidFileNames = 0
        for (var i=0; i < fileModel.count; i++)
        {
            var fileName = fileModel.get(i).newName

            if (!validateFileName(fileName))
                invalidFileNames += 1
        }

        if (invalidFileNames > 0)
            dialog.canAccept = false
        else
            dialog.canAccept = true
    }

    /*
     *  Check if the file name is valid
     */
    function validateFileName(fileName)
    {
        if (fileName.indexOf("\\") != -1 || fileName.indexOf("/") != -1)
            return false
        else
            return true
    }

    function updateFileName(id, newName)
    {
        fileModel.get(id).newName = newName
    }
}
