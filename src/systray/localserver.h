/****************************************************************************
**
** Copyright (C) 2015 Sagar Uday Kumar - Abstrium SAS <team (at) pyd.io>
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
#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QMainWindow>
#include <QObject>
#include <globals.h>
#include <JSEventHandler.h>

QT_BEGIN_NAMESPACE
class QLocalServer;
QT_END_NAMESPACE

class JSEventHandler;

class LocalServer : public QObject
{
    Q_OBJECT
    //static QString fileName;

public:
    LocalServer(QObject *parent = 0);
    QString fileName;

signals:
    void share(QString, QString, QString);

private slots:
    void GenerateLink();

private:
    QLocalServer *localserver;
    JSEventHandler *jseventhandler;
};

#endif // LOCALSERVER_H
