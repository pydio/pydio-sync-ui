#include "portconfigurer.h"

PortConfigurer::PortConfigurer(QString pathToFile)
{
    ipAddress = "tcp://127.0.0.1:";
    getPortsFromFile(pathToFile);

}

void PortConfigurer::getPortsFromFile(QString pathToFile)
{
    portHash.clear();
    QFile configFile(pathToFile);

    if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning("Cannot open file, check the path you entered.");
        setPortsToDefault();
        return;
    }

    QTextStream in(&configFile);
    if(!in.readLine().startsWith("Pydio port config file")){
        setPortsToDefault();
        return;
    }
    while(!in.atEnd()){
        QStringList line = in.readLine().split(":", QString::SkipEmptyParts);
        portHash[line[0]] = line[1];
    }
    configFile.close();
}

void PortConfigurer::setPortsToDefault()
{
    portHash.clear();
    portHash["pub_socket"] = QString::number(DEFAULT_PORT);
    portHash["command_socket"] = QString::number(DEFAULT_PORT + 1);
    portHash["watch_socket"] = QString::number(DEFAULT_PORT + 2);
}

QString PortConfigurer::port(QString socketName)
{
    return portHash[socketName];
}

QString PortConfigurer::address(QString socketName)
{
    return ipAddress + port(socketName);
}
