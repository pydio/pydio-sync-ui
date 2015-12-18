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
#ifndef JSEVENTHANDLER_H
#define JSEVENTHANDLER_H

#include <QObject>
#include <QFileDialog>
#include <QDesktopServices>
#include <cmdhelper.h>

class JSEventHandler : public QObject
{
    Q_OBJECT

public:
    explicit JSEventHandler(QObject *parent = 0);

public slots:
    QString getPath();
    void openUrl(QString);
    void openLogs();
    void copyToClipBoard(QString value);
};


#endif // JSEVENTHANDLER_H
