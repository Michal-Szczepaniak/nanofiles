#include "filelist.h"

FileList::FileList(QObject *parent) :
    QObject(parent)
{
    m_showHiddenFiles = false;

    m_sortBy = "";
    m_sortOrder = "";
    m_dirOrder = "";

    QStringList imageFormats;
    imageFormats << "image" << "png" << "jpg" << "jpeg" << "gif" << "svg";

    QStringList videoFormats;
    videoFormats << "video" << "mpg" << "avi" << "mov" << "3gp" << "mp4" << "mkv" << "wmv";

    QStringList audioFormats;
    audioFormats << "audio" << "mp3" << "ogg" << "wav" << "flac";

    QStringList packageFormats;
    packageFormats << "package" << "apk" << "rpm";

    QStringList textFormats;
    textFormats << "text" << "txt" << "conf" << "xml";

    QStringList docFormats;
    docFormats << "doc" << "doc" << "docx";

    QStringList xlsFormats;
    xlsFormats << "doc" << "xls" << "xlsx";

    QStringList pdfFormats;
    pdfFormats << "doc" << "pdf";

    QStringList archiveFormats;
    archiveFormats << "tar.gz" << "gz" << "zip";

    fileFormats.append(imageFormats);
    fileFormats.append(videoFormats);
    fileFormats.append(audioFormats);
    fileFormats.append(packageFormats);
    fileFormats.append(textFormats);
    fileFormats.append(docFormats);
    fileFormats.append(xlsFormats);
    fileFormats.append(pdfFormats);
    fileFormats.append(archiveFormats);
}

/*
 *  Get the directory that's currently being viewed
 */
QString FileList::getCurrentDirectory()
{
    return m_currentPath;
}

/*
 *  Get the user's home directory path
 */
QString FileList::getHomePath()
{
    return QDir::homePath();
}

/*
 *  Call a worker to change the file list
 */
/*void FileList::updateFileList(const QString &path)
{
    if (m_currentPath != path)
    {
        m_currentPath = path;
        resetFileInfoEntryList();
    }

    if (m_fileInfoEntryListMap.contains(path))
    {
        fileInfoEntryListCreated(m_fileInfoEntryListMap.value(path), path, QStringList());
        return;
    }

    // If we don't have a file list yet, create one using a worker
    DirectoryWorker *directoryWorker = new DirectoryWorker();

    connect(directoryWorker, SIGNAL(fileInfoEntryListCreated(QList<FileInfoEntry*>, QString, QStringList)), this, SLOT(fileInfoEntryListCreated(QList<FileInfoEntry*>,QString,QStringList)));

    directoryWorker->getFileInfoEntryList(path);
    directoryWorker->setOrdering(m_sortBy, m_sortOrder, m_dirOrder);
    directoryWorker->setShowHiddenFiles(m_showHiddenFiles);

    directoryWorker->startWorker();
}

QList<FileInfoEntry*> FileList::getFileInfoEntryList(const QString &path, const QString &fileTypes)
{
    // This function assumes a full file info entry list already exists, if it doesn't,
    // simulate spontaneous nuclear explosion (TODO)
    if (!m_fileInfoEntryListMap.contains(path))
    {
        qDebug("File info entry list not found!");
        return QList<FileInfoEntry*>();
    }

    QStringList allowedFileTypes = fileTypes.split(",");

    if (allowedFileTypes.at(0) == "")
        allowedFileTypes.clear();
    else if (allowedFileTypes.at(0) == "all") // all is the same as image, audio, video, text
    {
        allowedFileTypes << "image" << "audio" << "video" << "text";
    }

    QList<FileInfoEntry*> fileInfoEntryList = m_fileInfoEntryListMap.value(path);

    if (allowedFileTypes.count() == 0)
        return fileInfoEntryList;

    QList<FileInfoEntry*> newFileInfoEntryList;

    for (int i=0; i < fileInfoEntryList.length(); i++)
    {
        FileInfoEntry *fileInfoEntry = fileInfoEntryList.at(i);

        if (allowedFileTypes.count() > 0 && fileInfoEntry->getFileName() == "..")
            continue;

        if (allowedFileTypes.contains(fileInfoEntry->getFileType()))
        {
            newFileInfoEntryList.append(fileInfoEntry);
        }
    }

    return newFileInfoEntryList;
}

QVariant FileList::getFileList(const QString &path, const QString &fileTypes)
{
    return QVariant::fromValue(toFileObjectList(getFileInfoEntryList(path, fileTypes)));
}*/

/*
 *  Called when the worker has finished creating the file info entry list or when one
 *  is retrieved from memory
 */
/*void FileList::fileInfoEntryListCreated(QList<FileInfoEntry*> fileInfoEntryList, QString path, QStringList containedFileTypes)
{
    m_currentDirFileTypes.clear();
    m_currentDirFileTypes = containedFileTypes;

    // Add it into memory if it hasn't been already
    if (m_fileInfoEntryListMap.count() >= 2)
        m_fileInfoEntryListMap.remove(m_fileInfoEntryListMap.keys().at(0));

    m_fileInfoEntryListMap.insert(path, fileInfoEntryList);

    emit fileListCreated(QVariant::fromValue(toFileObjectList(fileInfoEntryList)), path);
}

QList<QObject*> FileList::toFileObjectList(QList<FileInfoEntry*> fileInfoEntryList)
{
    QList<QObject*> fileObjectList;

    for (int i=0; i < fileInfoEntryList.length(); i++)
    {
        fileObjectList.append(fileInfoEntryList.at(i));
    }

    return fileObjectList;
}*/


bool FileList::containsFileType(const QString &fileType)
{
    return m_currentDirFileTypes.contains(fileType);
}

/*
 *  Get file permissions for a file as a string
 */
QString FileList::getFilePermissions(QString fullPath, bool update)
{
    if (update && m_permissionMap.contains(fullPath))
        m_permissionMap.remove(fullPath);

    if (m_permissionMap.contains(fullPath))
        return m_permissionMap.value(fullPath);
    else
    {
        QFileInfo fileInfo(fullPath);

        QString permissionString = Util::getPermissionString(fileInfo.permissions());
        m_permissionMap.insert(fullPath, permissionString);

        return permissionString;
    }
}

/*
 *  Get the last modification date for a file
 */
QString FileList::getLastModified(QString fullPath)
{
    if (m_lastModifiedMap.contains(fullPath))
        return m_lastModifiedMap.value(fullPath);
    else
    {
        QFileInfo fileInfo(fullPath);

        // Use day/month/year format because it makes more sense and anyone who's willing to tell
        // otherwise is wrong

        // Show only time if the file was modified on the same day
        bool sameDay = false;

        QDate currentDate = QDate::currentDate();

        if (currentDate.dayOfYear() == fileInfo.lastModified().date().dayOfYear() &&
            currentDate.year() == fileInfo.lastModified().date().year())
            sameDay = true;

        QString modifiedString;

        if (sameDay)
            modifiedString = fileInfo.lastModified().toString("hh:mm");
        else
            modifiedString = fileInfo.lastModified().toString("dd/MM/yyyy");

        m_lastModifiedMap.insert(fullPath, modifiedString);

        return modifiedString;
    }
}

void FileList::setShowHiddenFiles(const bool &showHiddenFiles)
{
    m_showHiddenFiles = showHiddenFiles;
    resetFileInfoEntryList();
}

void FileList::setSortBy(const QString &sortBy)
{
    m_sortBy = sortBy;
    resetFileInfoEntryList();
}

void FileList::setSortOrder(const QString &sortOrder)
{
    m_sortOrder = sortOrder;
    resetFileInfoEntryList();
}

void FileList::setDirOrder(const QString &dirOrder)
{
    m_dirOrder = dirOrder;
    resetFileInfoEntryList();
}

/*
 *  Reset the current path, forcing the file list to be retrieved again next time it's requested
 */
void FileList::resetFileInfoEntryList()
{
//    m_fileInfoEntryListMap.clear();
    m_currentDirFileTypes.clear();

    m_permissionMap.clear();
    m_lastModifiedMap.clear();
}
