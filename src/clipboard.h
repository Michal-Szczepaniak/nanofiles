#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QVariantMap>

class Clipboard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileOperation READ getFileOperation WRITE setFileOperation NOTIFY fileOperationChanged)
    Q_PROPERTY(QString clipboardDir READ getClipboardDir WRITE setClipboardDir NOTIFY clipboardDirChanged)
    Q_PROPERTY(int selectedFileCount READ getSelectedFileCount NOTIFY selectedFileCountChanged)
    Q_PROPERTY(int clipboardFileCount READ getClipboardFileCount NOTIFY clipboardFileCountChanged)
public:
    explicit Clipboard(QObject *parent = 0);

    Q_INVOKABLE int getClipboardFileCount() const;
    Q_INVOKABLE int getSelectedFileCount() const;

    Q_INVOKABLE void changeFileOperation(const QString &fileOperation, const QString &clipboardDir);

    Q_INVOKABLE bool clipboardContainsFile(const QString &fullPath);
    Q_INVOKABLE bool selectedFilesContainsFile(const QString &fullPath);

    // fileOperation
    Q_INVOKABLE void setFileOperation(const QString &fileOperation);
    Q_INVOKABLE QString getFileOperation() const;

    // clipboardDir
    Q_INVOKABLE void setClipboardDir(const QString &clipboardDir);
    Q_INVOKABLE QString getClipboardDir() const;

signals:
    void selectedFilesCleared();
    void selectedFileCountChanged(int selectedFileCount);

    void clipboardCleared();
    void clipboardFileCountChanged(int clipboardFileCount);

    void fileOperationChanged(const QString &fileOperation);
    void clipboardDirChanged(const QString &clipboardDir);

public slots:
    Q_INVOKABLE void addFileToClipboard(const QString &fullPath);
    Q_INVOKABLE void removeFileFromClipboard(const QString &fullPath);
    Q_INVOKABLE QStringList getClipboard();
    Q_INVOKABLE void clearClipboard();

    Q_INVOKABLE void addFileToSelectedFiles(const QString &fullPath);
    Q_INVOKABLE void removeFileFromSelectedFiles(const QString &fullPath);
    Q_INVOKABLE QStringList getSelectedFiles();
    Q_INVOKABLE void clearSelectedFiles();

private:
    QString m_fileOperation;
    QString m_clipboardDir;

    QStringList m_selectedFiles;
    QStringList m_clipboardFiles;

};

#endif // CLIPBOARD_H
