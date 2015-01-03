#include "job.h"
#include <QDebug>

Job::Job(QObject *parent) :
    QObject(parent)
{
}

Job::Job(QString id, QString name, bool running, double eta, QString lastEventMessage, QString local, QString remote)
{
    this->id = id;
    this->name = name;
    this->remainingTime = eta;
    this->status = running;
    this->lastEventMessage = lastEventMessage;
    this->remote = QUrl(remote);
    this->hasWork = false;
    this->local = QUrl::fromLocalFile(local);
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
        QTime time = QDateTime::fromTime_t(this->remainingTime).toUTC().time();
        if(time.second() == 0){
            this->hasWork = false;
        }
        else{
            this->hasWork = true;
        }
        updated = true;
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
