#ifndef FILEPROCESS_H
#define FILEPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>

class FileProcess : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString file READ getFile WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString action READ getAction WRITE setAction NOTIFY actionChanged)
    Q_PROPERTY(double actionProgress READ getActionProgress WRITE setActionProgress NOTIFY actionProgressChanged)
    Q_PROPERTY(int exitCode READ getExitCode WRITE setExitCode NOTIFY exitCodeChanged)
    Q_PROPERTY(QString progressText READ getProgressText WRITE setProgressText NOTIFY progressTextChanged)
    Q_PROPERTY(bool actionRunning READ getActionRunning WRITE setActionRunning NOTIFY actionRunningChanged)
public:
    explicit FileProcess(QObject *parent = 0);

    Q_INVOKABLE void performFileAction(const QString &fullPath, const QString &action, const bool &track);

    // file
    void setFile(const QString &file);
    QString getFile() const;

    // action
    void setAction(const QString &action);
    QString getAction() const;

    // actionProgress
    void setActionProgress(const double &actionProgress);
    double getActionProgress() const;

    // exitCode
    void setExitCode(const int &exitCode);
    int getExitCode() const;

    // progressText
    void setProgressText(const QString &progressText);
    QString getProgressText() const;

    // actionRunning
    void setActionRunning(const bool &actionRunning);
    bool getActionRunning() const;

    QProcess *process;

public slots:
    void outputUpdated();
    void processFinished(int exitCode);

private:
    QString m_file;
    QString m_action;
    double m_actionProgress;
    QString m_progressText;
    int m_exitCode;
    bool m_actionRunning;

signals:
    void fileChanged(const QString &file);
    void actionChanged(const QString &action);
    void actionProgressChanged(const double &actionProgress);
    void exitCodeChanged(const int &exitCode);
    void progressTextChanged(const QString &progressText);
    void actionRunningChanged(const bool &actionRunning);

public slots:

};

#endif // FILEPROCESS_H
