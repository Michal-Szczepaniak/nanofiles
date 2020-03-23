#include "clipboard.h"

Clipboard::Clipboard(QObject *parent) :
    QObject(parent)
{
}

/*
 *  Change the file operation and move the selected files to the clipboard
 */
void Clipboard::changeFileOperation(const QString &fileOperation, const QString &clipboardDir)
{
    clearClipboard();

    for (int i=0; i < m_selectedFiles.count(); i++)
    {
        addFileToClipboard(m_selectedFiles.at(i));
    }

    clearSelectedFiles();

    setClipboardDir(clipboardDir);
    setFileOperation(fileOperation);
}

/*
 *  Whether the clipboard contains the file or not
 */
bool Clipboard::clipboardContainsFile(const QString &fullPath)
{
    return m_clipboardFiles.contains(fullPath);
}

bool Clipboard::selectedFilesContainsFile(const QString &fullPath)
{
    return m_selectedFiles.contains(fullPath);
}

/*
 *  Add a file to selected files
 */
void Clipboard::addFileToSelectedFiles(const QString &fullPath)
{
    if (!m_selectedFiles.contains(fullPath))
        m_selectedFiles.append(fullPath);

    emit selectedFileCountChanged(m_selectedFiles.count());
}

/*
 *  Remove a file from selected files
 */
void Clipboard::removeFileFromSelectedFiles(const QString &fullPath)
{
    m_selectedFiles.removeAll(fullPath);

    emit selectedFileCountChanged(m_selectedFiles.count());
}

QStringList Clipboard::getSelectedFiles()
{
    return m_selectedFiles;
}

/*
 *  Clear selected files
 */
void Clipboard::clearSelectedFiles()
{
    m_selectedFiles.clear();

    emit selectedFilesCleared();
    emit selectedFileCountChanged(m_selectedFiles.count());
}

/*
 *  Add a file to the clipboard
 */
void Clipboard::addFileToClipboard(const QString &fullPath)
{
    if (!m_clipboardFiles.contains(fullPath))
        m_clipboardFiles.append(fullPath);

    emit clipboardFileCountChanged(m_clipboardFiles.count());
}

/*
 *  Remove a file from the clipboard
 */
void Clipboard::removeFileFromClipboard(const QString &fullPath)
{
    if (m_clipboardFiles.contains(fullPath))
        m_clipboardFiles.removeAll(fullPath);

    emit clipboardFileCountChanged(m_clipboardFiles.count());
}

int Clipboard::getSelectedFileCount() const
{
    return m_selectedFiles.count();
}

int Clipboard::getClipboardFileCount() const
{
    return m_clipboardFiles.count();
}

QStringList Clipboard::getClipboard()
{
    return m_clipboardFiles;
}

/*
 *  Clear the clipboard
 */
void Clipboard::clearClipboard()
{
    for (int i=0; i < m_clipboardFiles.count(); i++)
    {
        qDebug() << m_clipboardFiles.at(i).toLatin1();
    }

    m_clipboardFiles.clear();
    setClipboardDir("");

    emit clipboardCleared();
    emit clipboardFileCountChanged(m_clipboardFiles.count());
}

/*
 *  fileOperation - file operation to be performed
 */
void Clipboard::setFileOperation(const QString &fileOperation)
{
    if (m_fileOperation != fileOperation)
    {
        m_fileOperation = fileOperation;
        emit fileOperationChanged(fileOperation);
    }
}

QString Clipboard::getFileOperation() const
{
    return m_fileOperation;
}

/*
 *  clipboardDir - the directory which contains the current clipboard entries
 */
void Clipboard::setClipboardDir(const QString &clipboardDir)
{
    if (m_clipboardDir != clipboardDir)
    {
        m_clipboardDir = clipboardDir;
        emit clipboardDirChanged(clipboardDir);
    }
}

QString Clipboard::getClipboardDir() const
{
    return m_clipboardDir;
}
