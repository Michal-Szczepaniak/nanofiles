#include "worker.h"

Worker::Worker(QObject *parent) :
    QThread(parent)
{
    m_fileOperation = None;
    m_progressText = "";
    m_currentEntry = "";
    m_progressValue = 0;
}

void Worker::run()
{
    // Perform file operation
    switch (m_fileOperation)
    {
        case Paste:
            pasteFiles(false);
            break;
        case CutPaste:
            pasteFiles(true);
            break;
        case Delete:
            deleteFiles();
            break;
        case None:
            break;
    }
}

void Worker::startPasteProcess(QStringList entryList, QString destination, QString clipboardDir, bool cut)
{
    clearFileLists();

    if (!cut)
        m_fileOperation = Paste;
    else
        m_fileOperation = CutPaste;

    m_entryList = entryList;
    m_destination = destination;
    m_clipboardDir = clipboardDir;

    start();
}

void Worker::startDeleteProcess(QStringList entryList)
{
    clearFileLists();

    m_fileOperation = Delete;
    m_entryList = entryList;

    start();
}

/*
 *  Paste files to destination
 */
void Worker::pasteFiles(bool cut)
{
    buildFileList();

    QList<QString> directoryList;

    QDir sourceDir(m_clipboardDir);

    QTime startTime = QTime::currentTime();

    emit progressTextChanged(QString(tr("Copying files...")));

    // Create a list of directories to be created
    for (int i=0; i < m_directoryList.count(); i++)
    {
        QString sourcePath = m_directoryList.at(i);
        QString newDirPath = QString("%1/%2").arg(m_destination, sourceDir.relativeFilePath(sourcePath));

        qDebug() << sourcePath.toLatin1() << " > " << newDirPath.toLatin1();

        directoryList.append(newDirPath);
    }

    emit progressTextChanged(tr("Creating directories..."));

    qDebug() << "FILES";

    // Create a list of files to be copied
    for (int i=0; i < m_fileList.count(); i++)
    {
        QString sourcePath = m_fileList.at(i);
        QString newFilePath = QString("%1/%2").arg(m_destination, sourceDir.relativeFilePath(sourcePath));

        qDebug() << sourcePath.toLatin1() << " > " << newFilePath.toLatin1();

        m_fileMap.insert(sourcePath, newFilePath);
    }

    // First, create the directories
    for (int i = 0; i < directoryList.count(); i++)
    {
        QString newDir = directoryList.at(i);

        QDir dir(newDir);

        // If the directory already exists, skip it
        if (dir.exists())
            continue;

        bool success = dir.mkdir(newDir);

        if (!success)
            directoryErrorMap.insert(newDir, DirCreateError);
    }

    // If any of the directories couldn't be created, abort the process
    if (directoryErrorMap.count() > 0)
    {
        emit progressTextChanged(tr("Directories couldn't be created."));
        emit fileOperationFinished();

        // Abort the process
        quit();
    }

    // Now, copy the files
    int fileCount = 0;
    QMapIterator<QString, QString> i(m_fileMap);
    while (i.hasNext())
    {
        i.next();
        fileCount++;
        QString sourceFilePath = i.key();
        QString newFilePath = i.value();

        QFile sourceFile(sourceFilePath);

        // Report the progress, but only every 50 milliseconds to prevent the UI thread
        // from being flooded with signals
        if (QTime::currentTime().msecsTo(startTime) <= -50)
        {
            startTime = QTime::currentTime();

            emit progressTextChanged(QString(tr("Copying files (%1 of %2)...").arg(QString("%1").arg(fileCount),
                                                                                 QString("%1").arg(m_fileMap.count()))));
            emit currentEntryChanged(sourceFile.fileName());

            double progress = (double)(fileCount) / (double)m_fileMap.count();
            emit progressValueChanged(progress);
        }

        // Copy the file
        bool success = sourceFile.copy(sourceFilePath, newFilePath);

        if (!success)
            fileErrorMap.insert(newFilePath, sourceFile.error());
    }

    // If we don't have to cut files we are done
    if (!cut)
    {
        if (fileErrorMap.count() == 0 && directoryErrorMap.count() == 0)
            emit progressTextChanged(tr("All files were copied successfully."));
        else
            emit progressTextChanged(tr("All files couldn't be copied successfully."));

        emit fileOperationFinished();

        // Done, time to self-destruct
        quit();
    }
    else
    {
        fileCount = 0;

        if (fileErrorMap.count() == 0 && directoryErrorMap.count() == 0)
        {
            // We copied all files successfully, so delete the old ones now
            i.toFront();
            while (i.hasNext())
            {
                i.next();
                fileCount++;
                QString sourceFilePath = i.key();

                QFile sourceFile(sourceFilePath);

                // Report the progress, but only every 50 milliseconds to prevent the UI thread
                // from being flooded with signals
                if (QTime::currentTime().msecsTo(startTime) <= -50)
                {
                    startTime = QTime::currentTime();

                    emit progressTextChanged(QString(tr("Deleting old files (%1 of %2)...").arg(QString("%1").arg(fileCount),
                                                                                         QString("%1").arg(m_fileMap.count()))));
                    emit currentEntryChanged(sourceFile.fileName());

                    double progress = (double)(fileCount) / (double)m_fileMap.count();
                    emit progressValueChanged(progress);
                }

                // Delete the file
                bool success = sourceFile.remove();

                if (!success)
                    fileErrorMap.insert(sourceFilePath, sourceFile.error());
            }

            emit progressTextChanged(QString(tr("Deleting old directories...")));
            emit progressValueChanged(-1);

            // Then delete the directories
            for (int i=m_directoryList.count()-1; i >= 0; i--)
            {
                QString sourcePath = m_directoryList.at(i);

                QDir dir(sourcePath);

                // If it doesn't exist, it was most likely removed earlier
                if (!dir.exists())
                    continue;

                bool success = dir.rmdir(sourcePath);

                if (!success)
                    directoryErrorMap.insert(sourcePath, DirDeleteError);
            }

            // Check if files were copied and deleted successfully
            if (fileErrorMap.count() == 0 && directoryErrorMap.count() == 0)
                emit progressTextChanged(tr("All of the files were cut and pasted successfully."));
            else
                emit progressTextChanged(tr("All of the files couldn't be cut and pasted successfully."));

            emit fileOperationFinished();

            // Done, time to self-destruct
            quit();
        }
        else
        {
            // All files couldn't be copied, so delete the new copied files to revert the process
            i.toFront();
            while (i.hasNext())
            {
                i.next();
                fileCount++;
                QString newFilePath = i.value();

                QFile newFile(newFilePath);

                // Report the progress, but only every 50 milliseconds to prevent the UI thread
                // from being flooded with signals
                if (QTime::currentTime().msecsTo(startTime) <= -50)
                {
                    startTime = QTime::currentTime();

                    emit progressTextChanged(QString(tr("Failed to copy all files, deleting copied files (%1 of %2)...").arg(QString("%1").arg(fileCount),
                                                                                         QString("%1").arg(m_fileMap.count()))));
                    emit currentEntryChanged(newFile.fileName());

                    double progress = (double)(fileCount) / (double)m_fileMap.count();
                    emit progressValueChanged(progress);
                }

                // Delete the file
                newFile.remove();
            }

            emit progressTextChanged(QString(tr("Failed to copy all files, deleting created directories...")));
            emit progressValueChanged(-1);

            // Delete the directories
            for (int i=directoryList.count()-1; i >= 0; i--)
            {
                QString newDir = directoryList.at(i);

                QDir dir(newDir);

                // If the directory already exists, skip it
                if (dir.exists())
                    continue;

                bool success = dir.rmdir(newDir);

                if (!success)
                    directoryErrorMap.insert(newDir, DirDeleteError);
            }

            emit progressTextChanged(tr("All files couldn't be copied successfully."));

            emit fileOperationFinished();
        }
    }
}

/*
 *  Delete files in the clipboard
 */
void Worker::deleteFiles()
{
    buildFileList();

    emit progressTextChanged(QString(tr("Deleting files...")));

    QTime startTime = QTime::currentTime();

    // Start deleting files
    for (int i=0; i < m_fileList.count(); i++)
    {
        QString fullPath = m_fileList.at(i);

        QFile file(fullPath);

        // Report the progress
        if (QTime::currentTime().msecsTo(startTime) <= -50)
        {
            startTime = QTime::currentTime();
            emit progressTextChanged(QString(tr("Deleting files (%1 of %2)...")).arg(QString("%1").arg(i+1),
                                                                                 QString("%1").arg(m_fileList.count())));
            emit currentEntryChanged(file.fileName());

            double progress = (double)(i + 1) / (double)m_fileList.count();
            emit progressValueChanged(progress);
        }

        file.open(QIODevice::WriteOnly);

        // Remove the file
        bool success = file.remove();

        if (!success)
            fileErrorMap.insert(fullPath, file.error());
    }

    // And then delete directories
    // Reverse order so that the subdirectories are deleted first
    for (int i=m_directoryList.count()-1; i >= 0; i--)
    {
        QString fullPath = m_directoryList.at(i);

        QDir dir(fullPath);

        // If it doesn't exist, it was most likely removed earlier
        if (!dir.exists())
            continue;

        bool success = dir.rmdir(fullPath);

        if (!success)
            directoryErrorMap.insert(fullPath, DirDeleteError);

        if (QTime::currentTime().msecsTo(startTime) <= -50)
        {
            startTime = QTime::currentTime();
            emit progressTextChanged(QString(tr("Deleting directories...")));
            emit currentEntryChanged(dir.dirName());
            emit progressValueChanged(-1);
        }
    }

    // We are done
    if (fileErrorMap.count() == 0 && directoryErrorMap.count() == 0)
        emit progressTextChanged(tr("All files were deleted successfully."));
    else
        emit progressTextChanged(tr("All files couldn't be deleted successfully."));

    emit fileOperationFinished();

    // Done, time to self-destruct
    quit();
}

/*
 *  Builds a list of every directory and file out of the entries in the clipboard
 */
void Worker::buildFileList()
{
    for (int i=0; i < m_entryList.count(); i++)
    {
        QString entry = m_entryList.at(i);
        QFileInfo fileInfo(entry);

        if (fileInfo.isDir())
        {
            addDirectoryFiles(entry);
        }
        else
            m_fileList.append(entry);
    }

    // FILES
    qDebug() << "FILES";
    for (int i=0; i < m_fileList.count(); i++)
        qDebug() << m_fileList.at(i).toLatin1();

    // DIRECTORIES
    qDebug() << "DIRECTORIES";

    for (int i=0; i < m_directoryList.count(); i++)
        qDebug() << m_directoryList.at(i).toLatin1();
}

/*
 *  Add directories and files from specified directory
 */
void Worker::addDirectoryFiles(QString dirPath)
{
    m_directoryList.append(dirPath);

    QDir dir(dirPath);
    dir.setFilter(QDir::Hidden | QDir::AllEntries | QDir::NoDotAndDotDot); // All files need to be deleted before directories can be deleted

    QFileInfoList fileInfoList = dir.entryInfoList();

    for (int i=0; i < fileInfoList.count(); i++)
    {
        QFileInfo fileInfo = fileInfoList.at(i);
        QString absoluteFilePath = fileInfo.absoluteFilePath();

        // Take out redundant entries (why do they need to there in the first place?)
        if (absoluteFilePath == dirPath ||
            absoluteFilePath == dirPath.left(dirPath.lastIndexOf("/")) ||
            absoluteFilePath == "/.." || absoluteFilePath == "/")
            continue;

        if (m_directoryList.contains(absoluteFilePath) || m_fileList.contains(absoluteFilePath))
            continue;

        if (fileInfo.isDir())
        {
            addDirectoryFiles(absoluteFilePath);
        }
        else
            m_fileList.append(absoluteFilePath);
    }
}

/*
 *  Clear file lists
 */
void Worker::clearFileLists()
{
    m_entryList.clear();
    m_fileList.clear();
    m_directoryList.clear();

    fileErrorMap.clear();
    directoryErrorMap.clear();
}
