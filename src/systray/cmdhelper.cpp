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
    this->pathToWinAgent = path;
}

void CmdHelper::launchAgentMac(bool stopBeforeLaunch){
    QProcess process;
    QString processName = "launchctl";
    QStringList arguments;
    if(stopBeforeLaunch){
        arguments = QStringList() << "remove"<< "PydioSync";
        process.start(processName, arguments);
        process.waitForStarted();
        process.waitForFinished();
    }
    arguments = QStringList()<<"load"<<"-w"<<"/library/LaunchAgents/io.pyd.sync.launcher.plist";
    process.start(processName, arguments);
    process.waitForStarted();
    process.waitForFinished();
}

void CmdHelper::launchAgentWin(){
    QProcess::startDetached(pathToWinAgent, QStringList());
    QTimer::singleShot(1500, this, SIGNAL(winAgentLaunched()));
}

void CmdHelper::stopAgentMac(){
    QProcess process;
    QString processName = "launchctl";
    QStringList arguments = QStringList() << "remove"<< "PydioSync";
    process.start(processName, arguments);
    process.waitForStarted();
    process.waitForFinished();
    arguments = QStringList() << "remove"<< "PydioSyncUI";
    process.start(processName, arguments);
    process.waitForStarted();
    process.waitForFinished();
}
