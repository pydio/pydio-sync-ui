/****************************************************************************
**
** Copyright (C) 2007-2015 Charles du Jeu - Abstrium SAS <team (at) pyd.io>
** This file is part of Pydio.
**
** Pydio is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  Pydio is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Pydio.  If not, see <http://www.gnu.org/licenses/>.
**
**  The latest code can be found at <http://pyd.io/>.
**
****************************************************************************/
#include "portconfigurer.h"

PortConfigurer::PortConfigurer(QString pathToFile)
{
    ipAddress = AGENT_SERVER_URL;
    qDebug()<<"Will look for ports config in " + pathToFile;
    configFile = new QFile(pathToFile);
    api_username = "";
    api_password = "";
}

void PortConfigurer::setPortsToDefault()
{
    api_port = QString::number(DEFAULT_PORT);
}

QString PortConfigurer::port()
{
    return this->api_port;
}

QString PortConfigurer::username()
{
    return this->api_username;
}

QString PortConfigurer::password()
{
    return this->api_password;
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
        // line is pydio:PORT:USER:PASSWORD
        QStringList line = in.readLine().split(":", QString::SkipEmptyParts);
        api_port = line[1];
        if(line.size() == 4){
            api_username = line[2];
            api_password = line[3];
        }
    }
    configFile->close();
}
