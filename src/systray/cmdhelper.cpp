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
#include "cmdhelper.h"

CmdHelper::CmdHelper(QObject *parent, QString path) :
    QObject(parent)
{
    this->pathToAgent = path;
#ifdef Q_OS_WIN
            QString agentPath = QDir::currentPath() + AGENT_FILE_NAME_WIN;
            if(path != ""){
                agentPath = path;
            }
            qDebug()<<"Agent process will be launched at" + agentPath;
            this->pathToAgent = agentPath;
#endif
#ifdef Q_OS_LINUX
            QString agentPath = QFileInfo( QCoreApplication::applicationFilePath() ).dir().path()  + AGENT_FILE_NAME_LINUX;
            if(path != ""){
                agentPath = path;
            }
            QFileInfo checkPath(agentPath);
            if(checkPath.exists() && checkPath.isFile()){
                qDebug()<<"Should try to start agent at " + agentPath;
                this->pathToAgent = agentPath;
            }else{
                qDebug()<<"Cannot find file " + agentPath + ", please start agent manually.";
            }
#endif
}

void CmdHelper::launchAgentMac(bool stopBeforeLaunch){
    QProcess process;
    QString processName = "launchctl";
    QStringList arguments;
    if(stopBeforeLaunch){
        arguments = QStringList() << "remove"<< MAC_SERVICE_NAME_AGENT;
        process.start(processName, arguments);
        process.waitForStarted();
        process.waitForFinished();
    }
    arguments = QStringList()<<"load"<<"-w"<<"/Library/LaunchAgents/"+MAC_SERVICE_FILE_AGENT;
    process.start(processName, arguments);
    process.waitForStarted();
    process.waitForFinished();
}

void CmdHelper::launchAgentProcess(){
    if(this->pathToAgent != ""){
        QProcess::startDetached(pathToAgent, QStringList());
        QTimer::singleShot(1500, this, SIGNAL(winAgentLaunched()));
    }
}

void CmdHelper::stopAgentMac(){
    QProcess process;
    QString processName = "launchctl";
    QStringList arguments = QStringList() << "remove"<< MAC_SERVICE_NAME_AGENT;
    process.start(processName, arguments);
    process.waitForStarted();
    process.waitForFinished();
    arguments = QStringList() << "remove"<< MAC_SERVICE_NAME_UI;
    process.start(processName, arguments);
    process.waitForStarted();
    process.waitForFinished();
}
