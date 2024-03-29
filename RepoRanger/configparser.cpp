#include "configparser.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

ManagerConfig ConfigParser::parseManagerConfig(const QString& filePath) {
    ManagerConfig config;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open manager config file:" << filePath;
        return config;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(fileData));
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in manager config file.";
        return config;
    }

    QJsonObject obj = doc.object();
    config.githubRepo = obj["githubRepo"].toString();
    config.githubBranch = obj["githubBranch"].toString();
    config.projectStartScript = obj["projectStartScript"].toString();
    config.projectConfigPath = obj["projectConfigPath"].toString();
    config.checkFrequency = obj["checkFrequency"].toInt(60000);

    return config;
}

ProjectConfig ConfigParser::parseProjectConfig(const QString& filePath) {
    ProjectConfig config;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open project config file:" << filePath;
        return config;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(fileData));
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in project config file.";
        return config;
    }

    QJsonObject obj = doc.object();
    config.canRestart = obj["canRestart"].toBool(false); // Default to false if not specified
    config.otherConfigVariable = obj["otherConfigVariable"].toString();

    return config;
}
