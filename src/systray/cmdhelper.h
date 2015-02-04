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
#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QTimer>
#include <QStandardPaths>
#include <QFileInfo>
#include <globals.h>

class CmdHelper : public QObject
{
    Q_OBJECT
public:
    explicit CmdHelper(QObject *parent = 0, QString path = "");
    static QString getAppDataDir(){
        #ifdef Q_OS_WIN
            QProcessEnvironment a = QProcessEnvironment::systemEnvironment();
            return a.value("APPDATA") + '\\' + PYDIO_DATA_DIR;
        //return QFileInfo(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).path() + PYDIO_DATA_DIR;
        #endif
        #ifdef Q_OS_MAC
            return QFileInfo(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).path() + PYDIO_DATA_DIR;
        #endif
    }

signals:
    void winAgentLaunched();

public slots:
    void launchAgentMac(bool stopBeforeLaunch = false);
    void launchAgentWin();
    void stopAgentMac();

private:
    QString pathToWinAgent;
};

#endif // CMDHELPER_H
