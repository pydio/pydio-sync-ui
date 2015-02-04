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
#include "jobaction.h"

jobAction::jobAction(QObject *parent, Job *job) :
    QAction(parent)
{
    this->job = job;
    if (this->job != NULL)
            this->setText(job->getName() + " - " + job->getJobDescription());
}

void jobAction::setJob(Job* newJob){
    this->job = newJob;
}

Job* jobAction::getJob(){
    return this->job;
}

void jobAction::update(){
    if (this->job != NULL)
        this->setText(job->getName() + " - " + job->getJobDescription());
}
