#include "customtrayicon.h"

CustomTrayIcon::CustomTrayIcon(QObject* parent) : QSystemTrayIcon(parent)
{
    this->syncAgentUp = false;
    this->setIcon(QIcon(":/images/Pydio16.png"));
    this->createMainMenu();
    this->jobMenus = new QHash<QString, JobMenu*>();
    this->globalRunningStatus = false;
    this->singleJob = NULL;
    separatorAction = new QAction(this);
    separatorAction->setSeparator(true);
    singleJobLocal = new QAction("Open local folder", this);
    singleJobLocal->setIcon(QIcon(":/images/folder.png"));
    connect(singleJobLocal, SIGNAL(triggered()), this, SLOT(openSingleJobLocal()));
    singleJobRemote = new QAction("Open remote", this);
    singleJobRemote->setIcon(QIcon(":/images/world.png"));
    connect(singleJobRemote, SIGNAL(triggered()), this, SLOT(openSingleJobRemote()));
    this->checkJobs();
}

void CustomTrayIcon::onNewJob(Job* job){
    if(!this->syncAgentUp){
        this->connectionMade();
    }
    //qDebug()<<"Should create job for "<<job->getName();
    if(jobMenus->empty() && singleJob == NULL){
        //qDebug()<<"SINGLE JOB";
        this->contextMenu()->removeAction(noJobAction);
        this->insertSingleJob(job);
    }
    else if(jobMenus->size() == 0 && singleJob != NULL){
        //qDebug()<<"SHOULD DELETE SINGLEJOB AND CREATE MENUS";
        this->insertNewJobMenu(singleJob->getJob());
        this->removeSingleJob();
        this->insertNewJobMenu(job);
    }
    else{
        this->insertNewJobMenu(job);
    }
    this->checkJobs();
}

void CustomTrayIcon::insertNewJobMenu(Job* newJob){
    //qDebug()<<"inserting new job";
    JobMenu *newJobMenu = new JobMenu(0, newJob);
    jobMenus->insert(newJob->getId(), newJobMenu);
    this->contextMenu()->insertMenu(settingsAction, newJobMenu);
}

void CustomTrayIcon::onJobUpdate(QString id){
    //qDebug()<<"on jobupdate";
    if(singleJob == NULL){
        jobMenus->operator [](id)->update();
    } else {
        singleJob->update();
    }
    this->checkJobs();
}

void CustomTrayIcon::jobsCleared(){
    if(!jobMenus->empty()){
        foreach(const QString &k, jobMenus->keys()){
            this->contextMenu()->removeAction(jobMenus->value(k)->menuAction());
        }
        jobMenus->clear();
    }
    else if(singleJob != NULL){
        this->removeSingleJob();
    }
}

void CustomTrayIcon::onJobDeleted(QString id){
    if(singleJob)
    {
        this->removeSingleJob();
        this->contextMenu()->insertAction(settingsAction, noJobAction);
    }
    else if(jobMenus->size() == 2){
        this->contextMenu()->removeAction(jobMenus->value(id)->menuAction());
        jobMenus->remove(id);
        JobMenu *jobToConvert = jobMenus->begin().value();
        this->contextMenu()->removeAction(jobToConvert->menuAction());
        this->insertSingleJob(jobToConvert->getJob());
        jobMenus->clear();
    }
    else{
        this->contextMenu()->removeAction(jobMenus->value(id)->menuAction());
        jobMenus->remove(id);
    }
    this->checkJobs();
}

void CustomTrayIcon::insertSingleJob(Job *job){
    singleJob = new jobAction(this, job);
    singleJob->setDisabled(true);
    this->contextMenu()->insertAction(settingsAction, singleJob);
    this->contextMenu()->insertAction(settingsAction, singleJobLocal);
    this->contextMenu()->insertAction(settingsAction, singleJobRemote);
    this->contextMenu()->insertAction(settingsAction, separatorAction);
}

void CustomTrayIcon::removeSingleJob(){
    this->contextMenu()->removeAction(singleJob);
    this->contextMenu()->removeAction(singleJobLocal);
    this->contextMenu()->removeAction(singleJobRemote);
    this->contextMenu()->removeAction(separatorAction);
    singleJob = NULL;
}

void CustomTrayIcon::checkJobs(){
    if(!jobMenus->empty()){
        QList<JobMenu*> jMenus = jobMenus->values();
        globalRunningStatus = jMenus[0]->getJob()->getStatus();
        for(int i=0; i<jMenus.size(); i++){
            Job *j = jMenus[i]->getJob();
            globalRunningStatus = globalRunningStatus || j->getStatus();
        }
    }
    else if(this->singleJob){
        globalRunningStatus = singleJob->getJob()->getStatus();
    }
    if(!this->singleJob && jobMenus->empty()){
        this->contextMenu()->insertAction(settingsAction,noJobAction);
        resumePauseSyncAction->setDisabled(true);
    }
    else{
        this->contextMenu()->removeAction(noJobAction);
        resumePauseSyncAction->setDisabled(false);
    }
    if(globalRunningStatus){
        resumePauseSyncAction->setText(tr("Pause sync"));
        resumePauseSyncAction->disconnect();
        connect(resumePauseSyncAction, SIGNAL(triggered()), this, SIGNAL(pauseSync()));
    }
    else{
        resumePauseSyncAction->setText(tr("Resume sync"));
        resumePauseSyncAction->disconnect();
        connect(resumePauseSyncAction, SIGNAL(triggered()), this, SIGNAL(resumeSync()));
    }
}

void CustomTrayIcon::connectionMade(){
    if(!this->syncAgentUp){
        this->syncAgentUp = true;
        this->jobsCleared();
        this->contextMenu()->removeAction(noAgentAction);
        this->contextMenu()->insertAction(settingsAction, noJobAction);
        settingsAction->setDisabled(false);
        this->contextMenu()->insertAction(aboutAction, resumePauseSyncAction);
    }
}

void CustomTrayIcon::connectionLost(){
    if(this->syncAgentUp){
        this->syncAgentUp = false;
        this->jobsCleared();
        this->contextMenu()->removeAction(noJobAction);
        this->contextMenu()->insertAction(settingsAction, noAgentAction);
        settingsAction->setDisabled(true);
        this->contextMenu()->removeAction(resumePauseSyncAction);
    }
}

void CustomTrayIcon::createMainMenu(){
    this->createActions();
    mainMenu = new QMenu(0);
    mainMenu->addSeparator();
    mainMenu->addAction(settingsAction);
    mainMenu->insertAction(settingsAction, noAgentAction);
    mainMenu->addSeparator();
    mainMenu->addAction(aboutAction);
    mainMenu->addAction(quitAction);
    this->setContextMenu(mainMenu);
}

void CustomTrayIcon::createActions(){
    settingsAction = new QAction(tr("Open Pydio"), this);
    connect(settingsAction, SIGNAL(triggered()), this->parent(), SLOT(show()));
    settingsAction->setDisabled(true);

    noJobAction = new QAction(tr("No active job"), this);
    noJobAction->setDisabled(true);

    noAgentAction = new QAction(tr("No active agent"), this);
    noAgentAction->setDisabled(true);

    resumePauseSyncAction = new QAction(tr("Pause sync"), this);
    connect(resumePauseSyncAction, SIGNAL(triggered()), this, SIGNAL(pauseSync()));

    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SIGNAL(about()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SIGNAL(quit()));
}

void CustomTrayIcon::openSingleJobLocal(){
    QDesktopServices::openUrl(this->singleJob->getJob()->getLocal());
}

void CustomTrayIcon::openSingleJobRemote(){
    QDesktopServices::openUrl(this->singleJob->getJob()->getRemote());
}
