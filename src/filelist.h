#ifndef FILELIST_H
#define FILELIST_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>

//#include "directoryworker.h"
//#include "fileinfoentry.h"
#include "util.h"

class FileList : public QObject
{
    Q_OBJECT
public:
    explicit FileList(QObject *parent = 0);

//    Q_INVOKABLE QVariant getFileList(const QString &path, const QString &fileTypes = QString(""));
//    QList<FileInfoEntry*> getFileInfoEntryList(const QString &path, const QString &fileTypes);

//    Q_INVOKABLE void updateFileList(const QString &path);

    Q_INVOKABLE QString getCurrentDirectory();
    Q_INVOKABLE QString getHomePath();

//    QList<QObject*> toFileObjectList(QList<FileInfoEntry*> fileInfoEntryList);

    Q_INVOKABLE bool containsFileType(const QString &fileType);

    Q_INVOKABLE QString getFilePermissions(QString fullPath, bool update = false);
    Q_INVOKABLE QString getLastModified(QString fullPath);

    // What file types the current dir contains
    QStringList m_currentDirFileTypes;

    // Permission and modification time lists to be used with list view
    QMap<QString, QString> m_permissionMap;
    QMap<QString, QString> m_lastModifiedMap;

    QString m_currentPath;

    // Frequently accessed objects to store in memory for quicker access
//    QMap<QString, QList<FileInfoEntry*> > m_fileInfoEntryListMap;

    QString m_fileTypes;

    bool m_showHiddenFiles;

    QString m_sortBy;
    QString m_sortOrder;
    QString m_dirOrder;

private:
    QList<QStringList> fileFormats;

signals:
    void currentDirectoryChanged(const QString &currentDirectory);

    void fileListCreated(QVariant newFileList, QString path);

public slots:
//    void fileInfoEntryListCreated(QList<FileInfoEntry*> fileInfoEntryList, QString path, QStringList containedFileTypes);

    void setShowHiddenFiles(const bool &showHiddenFiles);

    void setSortBy(const QString &sortBy);
    void setSortOrder(const QString &sortOrder);
    void setDirOrder(const QString &dirOrder);

    void resetFileInfoEntryList();

};

#endif // FILELIST_H
