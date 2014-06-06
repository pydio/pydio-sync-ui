#include "job.h"
#include <QDebug>

Job::Job(QObject *parent) :
    QObject(parent)
{
}

Job::Job(QString id, QString name, bool running, double eta)
{
    this->id = id;
    this->name = name;
    this->remainingTime = eta;
    this->status = running;
}

void Job::update(QString newName, bool newStatus, double eta)
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
    return this->name.split('/').last() + " : " + ((this->status)?((this->remainingTime<0)?"idle":("syncing (" + QString::number(this->remainingTime)) + "s remaining)"):"paused");
}
