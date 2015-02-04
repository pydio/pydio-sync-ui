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
#ifndef JOBMENU_H
#define JOBMENU_H

#include <QMenu>
#include <QAction>
#include <job.h>
#include <QDesktopServices>
#include <QUrl>

class JobMenu : public QMenu
{
    Q_OBJECT
public:
    explicit JobMenu(QWidget *parent = 0, Job *job = 0);
    Job* getJob();
    void update();

private:
    QAction *status;
    QAction *openLocalAction;
    QAction *openRemoteAction;
    Job *job;

public slots:
    void openLocal();
    void openRemote();

};

#endif // JOBMENU_H
