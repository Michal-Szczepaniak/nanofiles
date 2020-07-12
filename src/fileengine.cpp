#include "fileengine.h"
#include <unistd.h>
#include <sys/types.h>

FileEngine::FileEngine(QObject *parent) :
    QObject(parent)
{
    m_currentFileIndex = -1;
    m_rootMode = getuid() == 0;

    fileList = new FileList();
//    fileInfo = new FileInfo();
    fileProcess = new FileProcess();
    clipboard = new Clipboard();
//    settings = new Settings();
//    coverModel = new CoverModel();

//    fileList->setShowHiddenFiles(settings->getShowHiddenFiles());
//    fileList->setSortBy(settings->getSortBy());
//    fileList->setSortOrder(settings->getSortOrder());
//    fileList->setDirOrder(settings->getDirOrder());

//    connect(settings, SIGNAL(showHiddenFilesChanged(bool)), fileList, SLOT(setShowHiddenFiles(bool)));
//    connect(settings, SIGNAL(sortByChanged(QString)), fileList, SLOT(setSortBy(QString)));
//    connect(settings, SIGNAL(sortOrderChanged(QString)), fileList, SLOT(setSortOrder(QString)));
//    connect(settings, SIGNAL(dirOrderChanged(QString)), fileList, SLOT(setDirOrder(QString)));
//    connect(settings, SIGNAL(directoryViewSettingsChanged()), fileList, SLOT(resetFileInfoEntryList()));

//    connect(fileList, SIGNAL(currentDirectoryChanged(QString)), coverModel, SLOT(setCoverLabel(QString)));
}

/*void FileEngine::updateCurrentFileIndex(const QString &fullPath,
                                        const QString &path,
                                        const QString &fileTypes)
{
    QList<FileInfoEntry*> fileInfoList = fileList->getFileInfoEntryList(path, fileTypes);

    int currentFileIndex = getCurrentFileIndex();

    // If current file index is above 2, try
    if (currentFileIndex > 2)
    {
        for (int i=currentFileIndex-1; i < currentFileIndex+2; i++)
        {
            // Don't go out of bounds
            if (i <= fileInfoList.length() - 1 && i >= 0)
            {
                FileInfoEntry *fileInfoEntry = fileInfoList.at(i);

                if (fileInfoEntry->getFullPath() == fullPath)
                {
                    setCurrentFileIndex(i);
                    return;
                }
            }
        }
    }

    for (int i=0; i < fileInfoList.length(); i++)
    {
        FileInfoEntry *fileInfoEntry = fileInfoList.at(i);

        if (fileInfoEntry->getFullPath() == fullPath)
        {
            setCurrentFileIndex(i);
            return;
        }
    }
}*/

void FileEngine::performFileOperation(const QString &fileOperation,
                                      const QStringList &clipboardList,
                                      const QString &clipboardDir,
                                      const QStringList &selectedFiles,
                                      const QString &directory)
{
    Worker *worker = new Worker();

    connect(worker, SIGNAL(progressTextChanged(QString)), this, SIGNAL(progressTextChanged(QString)));
    connect(worker, SIGNAL(currentEntryChanged(QString)), this, SIGNAL(currentEntryChanged(QString)));
    connect(worker, SIGNAL(progressValueChanged(double)), this, SIGNAL(progressValueChanged(double)));

    connect(worker, SIGNAL(fileOperationFinished()), this, SIGNAL(fileOperationFinished()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    if (fileOperation == "paste")
    {
        bool cut = false;
        if (clipboard->getFileOperation() == "cut")
            cut = true;

        worker->startPasteProcess(clipboardList, directory, clipboardDir, cut);
    }
    else if (fileOperation == "delete")
    {
        worker->startDeleteProcess(selectedFiles);
    }
}

void FileEngine::resetCurrentFileIndex()
{
    setCurrentFileIndex(-1);
}

/*
 *  Rename files
 */
void FileEngine::renameFiles(const QString &jsonString)
{
    QVariantList fileList = QJsonDocument::fromJson(QByteArray(jsonString.toUtf8())).array().toVariantList();

    for (int i=0; i < fileList.length(); i++)
    {
        QMap<QString, QVariant> fileMap = fileList.at(i).toMap();

        QString sourceFullPath = fileMap.value("sourceFullPath").toString();
        QString sourceName = fileMap.value("sourceName").toString();
        QString newName = fileMap.value("newName").toString();

        // Load the file
        QFile file(sourceFullPath);

        file.rename(sourceFullPath.replace(sourceName, newName));
    }
}

/*
 *  Create tar.gz with files
 */
void FileEngine::tarFiles(const QString &tarFile, const QString &baseDir, const QStringList &fileList)
{
    QProcess *proc = new QProcess();
    QString exec = "tar";
    QStringList params;

    QString dir = baseDir;
    dir = dir.remove(QRegExp("/$")) + "/";
    QRegExp baseDirRe = QRegExp("^" + dir);

    params << "-c";
    params << "-z";
    params << "-C" << dir;
    params << "-f" << tarFile;
    for (int i=0; i < fileList.length(); i++)
    {
        QString file = fileList.at(i);
        params << file.remove(baseDirRe);
    }
    proc->start(exec, params);
}

/*
 *  Create files/directories based on the provided array
 */
void FileEngine::createEntries(const QString &jsonString)
{
    QVariantList entryList = QJsonDocument::fromJson(QByteArray(jsonString.toUtf8())).array().toVariantList();

    for (int i=0; i < entryList.length(); i++)
    {
        QMap<QString, QVariant> entryMap = entryList.at(i).toMap();

        QString type = entryMap.value("type").toString();
        QString path = entryMap.value("path").toString();
        QString name = entryMap.value("name").toString();

        // Create directories
        if (type == "directory")
        {
            QDir dir(path);
            dir.mkdir(QString("%1/%2").arg(path, name));
        }
        else if (type == "file")
        {
            QFile file(QString("%1/%2").arg(path, name));
            file.open(QFile::WriteOnly);
        }
    }
}

/*
 *  Copies a string to the clipboard
 */
void FileEngine::copyToClipboard(const QString &string)
{
    QClipboard *clipboard = QGuiApplication::clipboard();

    clipboard->setText(string);
}

/*
 *  Change a single file permission in a file
 */
bool FileEngine::changeFilePermission(QString fullPath, int permissionPos)
{
    QFile file(fullPath);

    QFileDevice::Permissions filePermissions = file.permissions();

    switch (permissionPos)
    {
        case 0:
            if (filePermissions & QFileDevice::ReadUser) filePermissions = filePermissions & ~QFileDevice::ReadUser;
            else filePermissions = filePermissions | QFileDevice::ReadUser;
            break;
        case 1:
            if (filePermissions & QFileDevice::WriteUser) filePermissions = filePermissions & ~QFileDevice::WriteUser;
            else filePermissions = filePermissions | QFileDevice::WriteUser;
            break;
        case 2:
            if (filePermissions & QFileDevice::ExeUser) filePermissions = filePermissions & ~QFileDevice::ExeUser;
            else filePermissions = filePermissions | QFileDevice::ExeUser;
            break;
        case 3:
            if (filePermissions & QFileDevice::ReadGroup) filePermissions = filePermissions & ~QFileDevice::ReadGroup;
            else filePermissions = filePermissions | QFileDevice::ReadGroup;
            break;
        case 4:
            if (filePermissions & QFileDevice::WriteGroup) filePermissions = filePermissions & ~QFileDevice::WriteGroup;
            else filePermissions = filePermissions | QFileDevice::WriteGroup;
            break;
        case 5:
            if (filePermissions & QFileDevice::ExeGroup) filePermissions = filePermissions & ~QFileDevice::ExeGroup;
            else filePermissions = filePermissions | QFileDevice::ExeGroup;
            break;
        case 6:
            if (filePermissions & QFileDevice::ReadOwner) filePermissions = filePermissions & ~QFileDevice::ReadOwner;
            else filePermissions = filePermissions | QFileDevice::ReadOwner;
            break;
        case 7:
            if (filePermissions & QFileDevice::WriteOwner) filePermissions = filePermissions & ~QFileDevice::WriteOwner;
            else filePermissions = filePermissions | QFileDevice::WriteOwner;
            break;
        case 8:
            if (filePermissions & QFileDevice::ExeOwner) filePermissions = filePermissions & ~QFileDevice::ExeOwner;
            else filePermissions = filePermissions | QFileDevice::ExeOwner;
            break;
    }

    bool success = file.setPermissions(filePermissions);

    return success;
}

/*
 *  Check if the SD card is mounted
 *  If it is, return the mount point path
 *  If not, return an empty string
 */
QString FileEngine::getSdCardMountPath()
{
    if (QDir("/run/user/media/sdcard").exists())
        return "/run/user/media/sdcard";
    else if (QDir("/media/sdcard").exists())
        return "/media/sdcard";
    else
        return "";
}

bool FileEngine::rootMode()
{
    return m_rootMode;
}

void FileEngine::setRootMode(bool rootMode)
{
    if (rootMode) {
        if (setuid(0)) {
            perror("setuid");
            exit(1);
        }
    } else {
        if (setuid(100000)) {
            perror("setuid");
            exit(1);
        }
    }

    m_rootMode = rootMode;
    emit rootModeChanged(rootMode);
}

/*
 *  currentFileIndex - the file index for the currently opened item
 */
void FileEngine::setCurrentFileIndex(const int &currentFileIndex)
{
    if (m_currentFileIndex != currentFileIndex)
    {
        m_currentFileIndex = currentFileIndex;
        emit currentFileIndexChanged(currentFileIndex);
    }
}

int FileEngine::getCurrentFileIndex() const
{
    return m_currentFileIndex;
}
