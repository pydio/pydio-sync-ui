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
#include "jobmenu.h"

JobMenu::JobMenu(QWidget *parent, Job *job) :
    QMenu(parent)
{
    this->job = job;
    if(this->job != NULL){
        this->setTitle(job->getName());
    }
    status = new QAction(job->getJobDescription(), this);
    status->setDisabled(true);
    openLocalAction = new QAction(tr("Open local folder"), this);
    openLocalAction->setIcon(QIcon(":/images/folder.png"));
    connect(openLocalAction, SIGNAL(triggered()), this, SLOT(openLocal()));
    openRemoteAction = new QAction(tr("Open remote"), this);
    openRemoteAction->setIcon(QIcon(":/images/world.png"));
    connect(openRemoteAction, SIGNAL(triggered()), this, SLOT(openRemote()));

    this->addAction(status);
    this->addAction(openLocalAction);
    this->addAction(openRemoteAction);
}

Job* JobMenu::getJob(){
    return this->job;
}

void JobMenu::update(){
    if (this->job != NULL){
        status->setText(this->job->getJobDescription());
    }
}

void JobMenu::openLocal(){
    QDesktopServices::openUrl(this->getJob()->getLocal());
}

void JobMenu::openRemote(){
    QDesktopServices::openUrl(this->getJob()->getRemote());
}
