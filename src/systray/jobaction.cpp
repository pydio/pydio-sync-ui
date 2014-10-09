#include "jobaction.h"

jobAction::jobAction(QObject *parent, Job *job) :
    QAction(parent)
{
    this->job = job;
    if (this->job != NULL)
            this->setText(job->getJobDescription());
}

void jobAction::setJob(Job* newJob){
    this->job = newJob;
}

Job* jobAction::getJob(){
    return this->job;
}

void jobAction::update(){
    if (this->job != NULL)
        this->setText(job->getJobDescription());
}
