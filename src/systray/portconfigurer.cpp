#include "portconfigurer.h"

PortConfigurer::PortConfigurer(QString pathToFile)
{
    ipAddress = "http://127.0.0.1:";
    configFile = new QFile(pathToFile);

}

void PortConfigurer::setPortsToDefault()
{
    api_port = QString::number(DEFAULT_PORT);
}

QString PortConfigurer::port()
{
    return this->api_port;
}

QString PortConfigurer::address()
{
    return ipAddress + port();
}

void PortConfigurer::updatePorts()
{
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
        api_port = line[1];
    }
    configFile->close();
}
