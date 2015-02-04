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
#include "job.h"
#include <QDebug>

Job::Job(QObject *parent) :
    QObject(parent)
{
}

Job::Job(QString id, QString name, bool running, bool hasWork, double eta, QString lastEventMessage, QString local, QString remote)
{
    this->id = id;
    this->name = name;
    this->remainingTime = eta;
    this->status = running;
    this->lastEventMessage = lastEventMessage;
    this->remote = QUrl(remote);
    this->hasWork = hasWork;
    this->local = QUrl::fromLocalFile(local);
}

void Job::update(QString newName, bool newStatus, bool hasWork, double eta, QString lastEventMessage)
{
    bool updated = false;
    if(this->name != newName){
        this->name = newName;
        updated = true;
    }
    if(this->status != newStatus){
        this->status = newStatus;
        updated = true;
    }
    if(this->remainingTime != eta){
        this->remainingTime = eta;
        updated = true;
    }
    if(this->hasWork != hasWork){
        this->hasWork = hasWork;
        emit this->updated(this->id);
    }
    if((this->lastEventMessage != lastEventMessage && (lastEventMessage != "Status: Paused"))){
        this->lastEventMessage = lastEventMessage;
        updated = true;
    }
    if(updated){
        emit this->updated(this->id);
    }
}

QString Job::getId()
{
    return this->id;
}

bool Job::getStatus(){
    return this->status;
}

QString Job::getName(){
    return this->name;
}

QUrl Job::getLocal(){
    return this->local;
}

QUrl Job::getRemote(){
    return this->remote;
}

bool Job::isSyncing(){
    return (hasWork && status);
}

QString Job::getJobDescription()
{
    QString desc;
    if(this->status){
        if(this->remainingTime <= 0){
            desc = "idle (" + this->lastEventMessage + ")";
        }
        else{
            desc = "syncing (" + this->remainingTimeToString() + " remaining)";
        }
    }
    else{
        desc = "Paused";
    }
    return desc;
}

QString Job::remainingTimeToString(){
    QTime time = QDateTime::fromTime_t(this->remainingTime).toUTC().time();
    if(time.hour() == 0){
        if(time.minute() == 0){
            return time.toString("ss's'");
        }
        return time.toString("mm'min':ss");
    }
    return time.toString("hh'h':mm:ss");
}
