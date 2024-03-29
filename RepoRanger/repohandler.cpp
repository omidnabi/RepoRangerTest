#include "repohandler.h"
#include "configparser.h"
#include <QCoreApplication>
#include <QRegularExpression>
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <thread>
#include <QFileInfo>

RepoHandler::RepoHandler(QObject *parent) : QObject(parent) {
    loadConfiguration(); // Ensure this is called first

    // Assuming managerConfig.checkFrequency is loaded and in milliseconds
    if (managerConfig.checkFrequency > 0) {
        connect(&updateTimer, &QTimer::timeout, this, &RepoHandler::checkForUpdates);
        updateTimer.start(managerConfig.checkFrequency); // Use the configured frequency
    } else {
        qCritical() << "Check frequency is not set properly in configuration.";
    }

    connect(&updateTimer, &QTimer::timeout, this, []() {
        qDebug() << "Timer ticked.";
    });
}


void RepoHandler::loadConfiguration() {
    QString configPath = QCoreApplication::applicationDirPath() + "/../managerConfig.json";
    ManagerConfig loadedConfig = ConfigParser::parseManagerConfig(configPath);
    if (loadedConfig.githubRepo.isEmpty()) {
        qCritical() << "GitHub repository URL is missing in manager config.";
        return; // Consider how you wish to handle critical failures like this
    }

    // Save the loaded configuration to the member variable
    managerConfig = loadedConfig;
    // No longer need to separately store these as they are part of managerConfig
    projectConfigPath = managerConfig.projectConfigPath;
    startScript = managerConfig.projectStartScript;
}

void RepoHandler::checkForUpdates() {
    if (managerConfig.githubRepo.isEmpty() || managerConfig.githubBranch.isEmpty()) {
        qCritical() << "Repository URL or branch name is missing in manager config.";
        return;
    }

    // Extract the directory path from the start script path
    QFileInfo startScriptInfo(startScript);
    QString scriptDir = startScriptInfo.absolutePath();

    qDebug() << "Current working directory before setting for gitProcess:" << QDir::currentPath();

    // Dynamically allocate the QProcess object for fetching updates
    QProcess* gitProcess = new QProcess(this);
    gitProcess->setWorkingDirectory(scriptDir); // Set the working directory to the script directory

    qDebug() << "Current working directory after setting for gitProcess:" << gitProcess->workingDirectory();

    // Connect the finished signal to a slot where we will check for updates after fetch
    connect(gitProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &RepoHandler::onGitFetchFinished);

    // Start fetching updates from the remote repository
    gitProcess->start("git", QStringList() << "fetch" << "--all");
}

int RepoHandler::parseGitProcessOutput(QProcess *process) {
    // Read the standard output from the process
    QByteArray output = process->readAllStandardOutput();
    // Convert the output to a string and trim whitespace
    QString outputStr = QString(output).trimmed();

    // Convert the string to an integer
    bool ok;
    int numCommits = outputStr.toInt(&ok);
    if (ok) {
        return numCommits; // Return the number of new commits
    } else {
        qCritical() << "Failed to parse git process output:" << outputStr;
        return -1; // Return -1 or another error indicator as appropriate
    }
}


void RepoHandler::onGitFetchFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    QProcess* gitProcess = qobject_cast<QProcess*>(sender());

    if (!gitProcess) {
        qCritical() << "Internal error: Failed to cast sender to QProcess.";
        return;
    }

    // Ensure the git fetch completed successfully
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        // Now that fetch has succeeded, count the new commits
        QProcess* countProcess = new QProcess(this);
        connect(countProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &RepoHandler::onGitCommitsCounted);
        countProcess->setWorkingDirectory(gitProcess->workingDirectory());
        countProcess->start("git", QStringList() << "rev-list" << "--count" << "HEAD..origin/" + managerConfig.githubBranch);
    } else {
        qCritical() << "Git fetch failed. Exit code:" << exitCode;
    }

    // Clean up the fetch process
    gitProcess->deleteLater();
}

void RepoHandler::onGitCommitsCounted(int exitCode, QProcess::ExitStatus exitStatus) {
    QProcess* countProcess = qobject_cast<QProcess*>(sender());

    if (!countProcess) {
        qCritical() << "Failed to cast sender to QProcess for commit count.";
        return;
    }

    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        int newCommits = parseGitProcessOutput(countProcess);
        if (newCommits > 0) {
            qDebug() << "New updates found. Preparing to restart...";
            prepareForRestart();
        } else if (newCommits == 0) {
            qDebug() << "No updates found.";
        } else {
            // Handle potential error from parseGitProcessOutput
            qCritical() << "Error parsing git process output.";
        }
    } else {
        qCritical() << "Error counting new commits. Exit code:" << exitCode;
    }

    // Clean up the count process
    countProcess->deleteLater();
}



void RepoHandler::onGitDiffFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    QProcess *gitProcess = qobject_cast<QProcess*>(sender());

    if (!gitProcess) {
        qCritical() << "Internal error: Failed to cast sender to QProcess.";
        return;
    }

    if (exitStatus == QProcess::NormalExit) {
        if (exitCode == 0) {
            qDebug() << "No updates found.";
        } else {
            qDebug() << "Updates found. Preparing to restart...";
            prepareForRestart();
        }
    } else {
        qCritical() << "Git diff failed. Exit code:" << exitCode;
    }
}





void RepoHandler::prepareForRestart() {
    // Before restarting, check if it's safe to do so
    if (isReadyForRestart()) {
        restartApplication();
    } else {
        qDebug() << "Restart aborted due to readiness check failure.";
    }
}

bool RepoHandler::isReadyForRestart() {
    bool readyForRestart = false;
    int checkCount = 0;
    const int maxChecks = 120; // Example: max 120 checks, with a 5-second delay, equals 10 minutes max
    while (!readyForRestart && checkCount < maxChecks) {
        ProjectConfig projectConfig = ConfigParser::parseProjectConfig(projectConfigPath);
        readyForRestart = projectConfig.canRestart;

        if (!readyForRestart) {
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait for 5 seconds before checking again
            ++checkCount;
        }
        else
        {
            qDebug() << "Can Restart:" << projectConfig.canRestart;
        }
    }
    if (checkCount >= maxChecks) {
        qDebug() << "Reached maximum checks for restart readiness without approval. Aborting restart.";
        return false;
    }
    return readyForRestart;
}


void RepoHandler::restartApplication() {
    qDebug() << "Restarting application...";

    // Split the start script into command and arguments
    static QRegularExpression re("\\s+");
    QStringList parts = startScript.split(re, Qt::SkipEmptyParts);
    if (parts.isEmpty()) {
        qCritical() << "Start script command is empty.";
        return;
    }

    QString program = parts.takeFirst(); // The first part is the program to run
    QStringList arguments = parts; // The rest are the arguments for the program

    // Use QFileInfo to get the directory of the start script
    QFileInfo startScriptInfo(startScript);
    QString scriptDir = startScriptInfo.absolutePath();

    // Run the start script in its directory
    qint64 pid; // Optionally capture the process ID
    if (!QProcess::startDetached(program, arguments, scriptDir, &pid)) {
        qCritical() << "Failed to restart the application.";
    }
    QCoreApplication::quit(); // Quit the current application
}

