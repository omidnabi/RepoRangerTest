#include <QCoreApplication>
#include <QDebug>
#include "configparser.h"
#include "repohandler.h"


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // The manager config file is located in the application directory
    QString managerConfigPath = QCoreApplication::applicationDirPath() + "/../managerConfig.json";
    ManagerConfig managerConfig = ConfigParser::parseManagerConfig(managerConfigPath);

    // The project config path is read from the managerConfig
    QString projectConfigPath =  managerConfig.projectConfigPath;
    ProjectConfig projectConfig = ConfigParser::parseProjectConfig(projectConfigPath);

    // Example usage of the parsed configuration
    qDebug() << "GitHub Repo:" << managerConfig.githubRepo;
    qDebug() << "Project Start Script:" << managerConfig.projectStartScript;
    qDebug() << "Can Restart:" << projectConfig.canRestart;


    // Initialize and use RepoHandler to manage repository updates and application restart
    RepoHandler repoHandler;
    repoHandler.checkForUpdates();


    return app.exec();
}
