#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <QString>

struct ManagerConfig {
    QString githubRepo;
    QString githubBranch;
    QString projectStartScript;
    QString projectConfigPath;
    int checkFrequency; // Frequency of checks in milliseconds
};

struct ProjectConfig {
    bool canRestart;
    QString otherConfigVariable; // Extend this struct as needed
};

class ConfigParser {
public:
    static ManagerConfig parseManagerConfig(const QString& filePath);
    static ProjectConfig parseProjectConfig(const QString& filePath);
};

#endif // CONFIGPARSER_H
