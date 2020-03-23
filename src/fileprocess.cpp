#include "fileprocess.h"

FileProcess::FileProcess(QObject *parent) :
    QObject(parent)
{
    m_file = "";
    m_action = "";
    m_actionProgress = -1;
    m_exitCode = 0;
    m_progressText = "";
    m_actionRunning = false;

    process = new QProcess();

    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(outputUpdated()));
    connect(process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
}

/*
 *  Perform a file action
 *
 *  If track is true, run the application disregarding whether anything actually happens
 *  If track is false, run the application and track it until it's finished
 */
void FileProcess::performFileAction(const QString &fullPath, const QString &action, const bool &track)
{
    QString program;
    QStringList parameters;

    if (action == "installApk")
    {
        program = "xdg-open";
        parameters << fullPath;
    }
    else if (action == "installRpm")
    {
        program = "xdg-open";
        parameters << fullPath;
    }
    else if (action == "openSystem")
    {
        program = "xdg-open";
        parameters << fullPath;
    }
    else if (action == "execute")
    {
        program = fullPath;
    }

    // Launch the application
    if (!track)
    {
        QProcess *tempProcess = new QProcess();

        // Remember to destroy the QProcess once it is complete
        connect(tempProcess, SIGNAL(finished(int)), tempProcess, SLOT(deleteLater()));

        tempProcess->start(program, parameters);
    }
    else
    {
        // We can't track two actions at once
        if (getActionRunning())
            return;

        qDebug() << "Starting it here";

        setFile(fullPath);
        setAction(action);
        setActionRunning(true);
        setActionProgress(-1);

        // Set suitable progress text
        if (action == "installApk" || action == "installRpm")
            setProgressText(tr("Installing..."));
        else if (action == "openSystem")
            setProgressText(tr("Opening file..."));

        process->start(program, parameters);
    }
}

// More output received from application
void FileProcess::outputUpdated()
{
    QByteArray output = process->readAll();

    QString action = getAction();

    // Get the output so we can parse it and provide the user
    // with progress
    if (action == "installRpm")
    {
        if (output.contains("Installing"))
            setProgressText(tr("Installing packages"));
        if (output.contains("Downloading"))
            setProgressText(tr("Downloading packages"));
        if (output.contains("Percentage"))
        {
            QString percentageString(output.right(5));
            percentageString.remove("\n");
            percentageString = percentageString.trimmed();
            double percentage = percentageString.toDouble();
            percentage /= 100;
            qDebug() << "percentage " << percentage;
            setActionProgress(percentage);
        }
    }
}

// Process finished
void FileProcess::processFinished(int exitCode)
{
    QString action = getAction();
    setExitCode(exitCode);

    // installRpm exit codes
    if (action == "installRpm")
    {
        switch (exitCode)
        {
            case 0:
                setProgressText(tr("Installed"));
                break;

            case 5:
                setProgressText(tr("Nothing was done"));
                break;

            default:
                setProgressText(tr("Installation failed"));
                break;
        }
    }

    // installApk exit codes
    if (action == "installApk")
    {
        switch (exitCode)
        {
            case 0:
                setProgressText(tr("Installation was started"));
                break;

            default:
                setProgressText(tr("Installation couldn't be started"));
                break;
        }
    }

    // openSystem
    if (action == "openSystem")
    {
        switch (exitCode)
        {
            case 0:
                setProgressText(tr("File opened"));
                break;

            case 3:
                setProgressText(tr("Suitable application was not found"));
                break;

            default:
                setProgressText(tr("File couldn't be opened"));
                break;
        }
    }

    setActionProgress(1);
    setActionRunning(false);
}

/*
 *  file - file the current action is bound to
 */
void FileProcess::setFile(const QString &file)
{
    if (m_file != file)
    {
        m_file = file;
        emit fileChanged(file);
    }
}

QString FileProcess::getFile() const
{
    return m_file;
}

/*
 *  action - the current action that is being performed
 */
void FileProcess::setAction(const QString &action)
{
    if (m_action != action)
    {
        m_action = action;
        emit actionChanged(action);
    }
}

QString FileProcess::getAction() const
{
    return m_action;
}

/*
 *  actionProgress - current progress of the action, -1 if action's progress is
 *  indeterminable
 */
void FileProcess::setActionProgress(const double &actionProgress)
{
    if (m_actionProgress != actionProgress)
    {
        m_actionProgress = actionProgress;
        emit actionProgressChanged(actionProgress);
    }
}

double FileProcess::getActionProgress() const
{
    return m_actionProgress;
}

/*
 *  exitCode - the exit code returned by the last executed executable
 */
void FileProcess::setExitCode(const int &exitCode)
{
    if (m_exitCode != exitCode)
    {
        m_exitCode = exitCode;
        emit exitCodeChanged(exitCode);
    }
}

int FileProcess::getExitCode() const
{
    return m_exitCode;
}

/*
 *  progressText - progress text that is shown to the user
 */
void FileProcess::setProgressText(const QString &progressText)
{
    if (m_progressText != progressText)
    {
        m_progressText = progressText;
        emit progressTextChanged(progressText);
    }
}

QString FileProcess::getProgressText() const
{
    return m_progressText;
}

/*
 *  actionRunning - is an action running
 */
void FileProcess::setActionRunning(const bool &actionRunning)
{
    if (m_actionRunning != actionRunning)
    {
        m_actionRunning = actionRunning;
        emit actionRunningChanged(actionRunning);
    }
}

bool FileProcess::getActionRunning() const
{
    return m_actionRunning;
}
