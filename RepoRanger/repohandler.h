#ifndef REPOHANDLER_H
#define REPOHANDLER_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include "configparser.h" // Ensure this is included for ManagerConfig and ProjectConfig

class RepoHandler : public QObject {
    Q_OBJECT

public:
    explicit RepoHandler(QObject *parent = nullptr);
    void checkForUpdates();
    void restartApplication();

private:
    void loadConfiguration();
    bool isReadyForRestart();
    void prepareForRestart();

    ManagerConfig managerConfig; // Add managerConfig here
    QString projectConfigPath;
    QString startScript;
    QTimer updateTimer; // QTimer for periodic checks

    int parseGitProcessOutput(QProcess *process);
private slots:
    void onGitFetchFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onGitDiffFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onGitCommitsCounted(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // REPOHANDLER_H
