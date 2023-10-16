#ifndef FILEENGINE_H
#define FILEENGINE_H

#include <QObject>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QClipboard>
#include <QDir>
#include "filelist.h"
#include "fileprocess.h"
#include "worker.h"
#include "clipboard.h"

class FileEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentFileIndex READ getCurrentFileIndex WRITE setCurrentFileIndex NOTIFY currentFileIndexChanged)
    Q_PROPERTY(bool rootMode READ getRootMode WRITE setRootMode NOTIFY rootModeChanged)
public:
    explicit FileEngine(QObject *parent = 0);

    Q_INVOKABLE void resetCurrentFileIndex();

    Q_INVOKABLE void performFileOperation(const QString &fileOperation,
                                          const QStringList &clipboardList,
                                          const QString &clipboardDir,
                                          const QStringList &selectedFiles,
                                          const QString &directory);

    Q_INVOKABLE void renameFiles(const QString &jsonString);
    Q_INVOKABLE void tarFiles(const QString &tarFile, const QString &baseDir, const QStringList &fileList);
    Q_INVOKABLE void createEntries(const QString &jsonString);

    Q_INVOKABLE void copyToClipboard(const QString &string);

    Q_INVOKABLE bool changeFilePermission(QString fullPath, int permissionPos);

    Q_INVOKABLE QString getSdCardMountPath();

    bool getRootMode();
    void setRootMode(bool rootMode);

    void setCurrentFileIndex(const int &currentFileIndex);
    int getCurrentFileIndex() const;

    FileList *fileList;
    FileProcess *fileProcess;
    Clipboard *clipboard;

private:
    int _currentFileIndex;
    bool _rootMode;

signals:
    void currentFileIndexChanged(const int &currentFileIndex);

    // File operation signals
    void progressTextChanged(const QString &progressText);
    void progressValueChanged(const double &progressValue);
    void currentEntryChanged(const QString &currentEntry);

    void fileOperationFinished();
    void rootModeChanged(bool rootMode);

public slots:

};

#endif // FILEENGINE_H
