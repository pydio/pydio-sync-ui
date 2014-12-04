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
