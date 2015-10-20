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
#include "JSEventHandler.h"
#include "globals.h"
#include <QDebug>
//#include <localserver.h>

JSEventHandler::JSEventHandler(QObject *parent) :
    QObject(parent)
{
}

QString JSEventHandler::getPath()
{
   QString pydioDir = QDir::homePath() + "/" + PYDIO_DATA_DIR;
   return QFileDialog::getExistingDirectory(0, tr("Select local path"), pydioDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

void JSEventHandler::openUrl(QString toOpen)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(toOpen));
}

void JSEventHandler::openLogs(){
    QString logDir = CmdHelper::getAppDataDir();
    QDesktopServices::openUrl(QUrl::fromLocalFile(logDir));
}

QString JSEventHandler::getFilePath()
{
   QString pydioDir = QDir::homePath() + "/" + PYDIO_DATA_DIR;
   return QFileDialog::getOpenFileName(0, tr("Select local path"), pydioDir,0);
}

QString JSEventHandler::getShareName()
{
   return shareFileName;
}
QString JSEventHandler::getShareJobId()
{
   return shareFileJobId;
}

void JSEventHandler::setFileName(QList<QString> value)
{
    shareFileName = value[1];
    shareFileJobId = value[0];
}
