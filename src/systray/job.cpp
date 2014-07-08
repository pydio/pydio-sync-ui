#include "job.h"
#include <QDebug>

Job::Job(QObject *parent) :
    QObject(parent)
{
}

Job::Job(QString id, QString name, bool running, double eta, QString lastEventMessage)
{
    this->id = id;
    this->name = name;
    this->remainingTime = eta;
    this->status = running;
    this->lastEventMessage = lastEventMessage;
}

void Job::update(QString newName, bool newStatus, double eta, QString lastEventMessage)
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
    if(this->lastEventMessage != lastEventMessage){
        this->lastEventMessage = lastEventMessage;
        updated = true;
    }
    if(updated){
        emit this->updated(this->id, this->getJobDescription());
    }
}

QString Job::getId()
{
    return this->id;
}

QString Job::getJobDescription()
{
    QString desc;
    if(this->status){
        if(this->remainingTime < 0){
            desc = "idle (" + this->lastEventMessage + ")";
        }
        else{
            desc = "syncing (" + QString::number(this->remainingTime, 'f', 1) + "s remaining)";
        }
    }
    else{
        desc = this->lastEventMessage;
    }
    return this->name + " - " + desc;
}
