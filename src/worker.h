#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QVariantMap>
#include <QTime>

class Worker : public QThread
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);

    Q_INVOKABLE void pasteFiles(bool cut = false);
    Q_INVOKABLE void deleteFiles();

    void addDirectoryFiles(QString dirPath);
    void buildFileList();
    void clearFileLists();

public slots:
    void startPasteProcess(QStringList entryList, QString destination, QString clipboardDir, bool cut);
    void startDeleteProcess(QStringList entryList);

signals:
    void progressTextChanged(const QString &progressText);
    void progressValueChanged(const double &progressValue);
    void currentEntryChanged(const QString &currentEntry);

    void fileOperationFinished();

protected:
    void run();

private:
    enum FileOperation {
        None, Delete, Paste, CutPaste
    };

    enum DirError {
        DirDeleteError, DirCreateError
    };

    FileOperation m_fileOperation;

    QString m_progressText;
    QString m_currentEntry;
    QString m_destination;
    QString m_clipboardDir;

    double m_progressValue;

    QStringList m_entryList;

    // Used when processing files
    QStringList m_fileList;
    QStringList m_directoryList;

    QMap<QString, QString> m_fileMap;
    QMap<QString, QString> m_directoryMap;

    QMap<QString, QFile::FileError> fileErrorMap;
    QMap<QString, DirError> directoryErrorMap;
};

#endif // WORKER_H
