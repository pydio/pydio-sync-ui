#ifndef PORTCONFIGURER_H
#define PORTCONFIGURER_H

#include <QHash>
#include <QFile>
#include <QTextStream>
#include <globals.h>


class PortConfigurer
{
public:
    PortConfigurer(QString pathToFile);
    QString port();
    QString address();
    void updatePorts();

private:
    void setPortsToDefault();
    QString ipAddress;
    QFile *configFile;
    QString api_port;
};

#endif // PORTCONFIGURER_H
