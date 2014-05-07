#ifndef PORTCONFIGURER_H
#define PORTCONFIGURER_H

#include <QHash>
#include <QFile>
#include <QTextStream>

class PortConfigurer
{
public:
    PortConfigurer(QString pathToFile);
    QString port(QString socketName);
    QString address(QString socketName);
    void getPortsFromFile(QString);

private:
    void setPortsToDefault();
    QHash<QString, QString> portHash;
    QString ipAddress;
    const static int DEFAULT_PORT = 5556;
};

#endif // PORTCONFIGURER_H
