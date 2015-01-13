#ifndef PORTCONFIGURER_H
#define PORTCONFIGURER_H

#include <QHash>
#include <QFile>
#include <QTextStream>

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
    const static int DEFAULT_PORT = 5556;
    QFile *configFile;
    QString api_port;
};

#endif // PORTCONFIGURER_H
