#include "portconfigurer.h"

PortConfigurer::PortConfigurer(QString pathToFile)
{
    ipAddress = "http://127.0.0.1:";
    configFile = new QFile(pathToFile);

}

void PortConfigurer::setPortsToDefault()
{
    portHash.clear();
    portHash["flask_api"] = QString::number(DEFAULT_PORT);
}

QString PortConfigurer::port(QString socketName)
{
    return portHash[socketName];
}

QString PortConfigurer::address(QString socketName)
{
    return ipAddress + port(socketName);
}

void PortConfigurer::updatePorts()
{
    portHash.clear();
    if(!configFile->open(QIODevice::ReadOnly | QIODevice::Text)){
        setPortsToDefault();
        return;
    }

    QTextStream in(configFile);
    if(!in.readLine().startsWith("Pydio port config file")){
        setPortsToDefault();
        return;
    }
    while(!in.atEnd()){
        QStringList line = in.readLine().split(":", QString::SkipEmptyParts);
        portHash[line[0]] = line[1];
    }
    configFile->close();
}
