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
#include "customtrayicon.h"

CustomTrayIcon::CustomTrayIcon(QObject* parent) : QSystemTrayIcon(parent)
{

#ifdef Q_OS_LINUX
    this->iconActive = QIcon(":/images/PydioSync-Systray-Linux.png");
    this->iconTransfer = QIcon(":/images/PydioSync-Systray-Linux-Transfer.png");
    this->iconInactive = QIcon(":/images/PydioSync-Systray-Linux-Inactive.png");
#else
    this->iconActive = QIcon(":/images/PydioSync-Systray-Mac.png");
    this->iconTransfer = QIcon(":/images/PydioSync-Systray-Mac-Transfer.png");
    this->iconInactive = QIcon(":/images/PydioSync-Systray-Mac-Inactive.png");
#endif

    this->debugMode = true;
    this->pathToWinAgent = pathToWinAgent;
    this->syncAgentUp = false;
    this->setIconNormal();
    this->createMainMenu();
    this->jobMenus = new QHash<QString, JobMenu*>();
    this->globalRunningStatus = false;
    this->singleJob = NULL;
    this->working = false;
    this->animationOn = false;
    this->normalIcon = false;
    this->agentConnectedToInternet = true;

    this->animationTimer = new QTimer(this);
    this->animationTimer->setInterval(1100);
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(changeIcon()));
    separatorAction = new QAction(this);
    separatorAction->setSeparator(true);
    singleJobLocal = new QAction(tr("Open local folder"), this);
    singleJobLocal->setIcon(QIcon(":/images/folder-32.png"));
    connect(singleJobLocal, SIGNAL(triggered()), this, SLOT(openSingleJobLocal()));
    singleJobRemote = new QAction(tr("Open remote"), this);
    singleJobRemote->setIcon(QIcon(":/images/world.png"));
    connect(singleJobRemote, SIGNAL(triggered()), this, SLOT(openSingleJobRemote()));
    this->checkJobs();
}

void CustomTrayIcon::onNewJob(Job* job){
    debug("Should create job for" + job->getName());
    if(!this->syncAgentUp){
        this->connectionMade();
    }
    if(jobMenus->empty() && singleJob == NULL){
        this->contextMenu()->removeAction(noJobAction);
        this->insertSingleJob(job);
    }
    else if(jobMenus->size() == 0 && singleJob != NULL){
        debug("SHOULD DELETE SINGLEJOB AND CREATE MENUS");
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
    //debug("inserting new job");
    JobMenu *newJobMenu = new JobMenu(0, newJob);
    jobMenus->insert(newJob->getId(), newJobMenu);
    this->contextMenu()->insertMenu(settingsAction, newJobMenu);
}

void CustomTrayIcon::onJobUpdate(QString id){
    debug("SHOULD UPDATE : " + id);
    if(singleJob == NULL){
        jobMenus->operator [](id)->update();
    } else {
        singleJob->update();
    }
    this->checkJobs();
}

void CustomTrayIcon::jobsCleared(QString reason){
    if(!jobMenus->empty()){
        foreach(const QString &k, jobMenus->keys()){
            this->contextMenu()->removeAction(jobMenus->value(k)->menuAction());
        }
        jobMenus->clear();
    }
    else if(singleJob != NULL){
        this->removeSingleJob();
    }
    debug("JOBS ARE CLEARED, REASON : " + reason);
}

void CustomTrayIcon::onJobDeleted(QString id){
    debug("SHOULD DELETE JOB : " + id);
    if(singleJob)
    {
        this->removeSingleJob();
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
    this->contextMenu()->removeAction(noJobAction);
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
    checkJobs();
}

void CustomTrayIcon::checkJobs(){
    if(!jobMenus->empty()){
        QList<JobMenu*> jMenus = jobMenus->values();
        globalRunningStatus = jMenus[0]->getJob()->getStatus();
        working = false;
        for(int i=0; i<jMenus.size(); i++){
            Job *j = jMenus[i]->getJob();
            globalRunningStatus = globalRunningStatus || j->getStatus();
            working = working || j->isSyncing();
        }
    }
    else if(this->singleJob){
        globalRunningStatus = singleJob->getJob()->getStatus();
        working = singleJob->getJob()->isSyncing();
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
    if(working){
        this->workOccuring();
    }
    else{
        this->workDone();
    }
}

void CustomTrayIcon::connectionMade(){
    if(!this->syncAgentUp){
        debug("SINGLE JOB IS " + (singleJob ? singleJob->getJob()->getName() : "NULL"));
        debug("NUMBER OF JOBS : " +  QString::number(this->jobMenus->size()));
        emit check_for_update();
        this->syncAgentUp = true;
        this->setIconNormal();
        this->normalIcon = true;
        this->jobsCleared("Connection Made");
        this->contextMenu()->removeAction(noAgentAction);
        this->contextMenu()->insertAction(settingsAction, noJobAction);
        settingsAction->setDisabled(false);
        //shareAction->setDisabled(false);
        this->contextMenu()->insertAction(aboutAction, resumePauseSyncAction);        
    }
}

void CustomTrayIcon::connectionLost(){
    if(this->syncAgentUp){
        working = false;
        animationTimer->stop();
        animationOn = false;
        this->syncAgentUp = false;
        this->setIconInactive();
        this->normalIcon = false;
        this->jobsCleared("CONNECTION LOST");
        debug("Working status : Off / Animation : Off / Normal Icon : on");
        this->contextMenu()->removeAction(noJobAction);
        this->contextMenu()->insertAction(settingsAction, noAgentAction);
        settingsAction->setDisabled(true);
        //shareAction->setDisabled(true);
        this->contextMenu()->removeAction(resumePauseSyncAction);
        debug("SINGLE JOB IS " + (singleJob ? singleJob->getJob()->getName() : "NULL"));
        debug("NUMBER OF JOBS : " +  QString::number(this->jobMenus->size()));
    }
}

void CustomTrayIcon::noInternetConnection(){
    if(agentConnectedToInternet){
        this->setIconInactive();
        this->contextMenu()->insertAction(settingsAction, noInternetAction);
        agentConnectedToInternet = false;
    }
}

void CustomTrayIcon::internetConnectionOk(){
    if(!agentConnectedToInternet){
        this->setIconNormal();
        this->contextMenu()->removeAction(noInternetAction);
        agentConnectedToInternet = true;
    }
}

void CustomTrayIcon::notificationReceived(QString jobId, QString type, QString message){
    QString jobTitle;
    if(singleJob == NULL && jobMenus->contains(jobId)){
        jobTitle = jobMenus->value(jobId)->getJob()->getName();
    } else {
        jobTitle = singleJob->getJob()->getName();
    }
    debug("NOTIFICATION RECEIVED: " + jobTitle + " - " + message);
    this->showMessage(jobTitle, message);
}

void CustomTrayIcon::createMainMenu(){
    this->createActions();
    mainMenu = new QMenu(0);
    mainMenu->addSeparator();
    mainMenu->addAction(settingsAction);
    mainMenu->insertAction(settingsAction, noAgentAction);
    //mainMenu->addAction(shareAction); // To be only enabled during debug
    mainMenu->addSeparator();
    mainMenu->addAction(aboutAction);    
    mainMenu->addAction(quitAction);
    this->setContextMenu(mainMenu);
}

void CustomTrayIcon::createActions(){
    settingsAction = new QAction(tr("Settings"), this);
    connect(settingsAction, SIGNAL(triggered()), this->parent(), SLOT(show()));
    settingsAction->setDisabled(true);

    noJobAction = new QAction(tr("No active job"), this);
    noJobAction->setDisabled(true);

    /*
#ifdef Q_OS_WIN
    noAgentAction = new QAction(tr("Launch agent"), this);
    connect(noAgentAction, SIGNAL(triggered()), this, SLOT(launchAgent()));
#else
*/
    noAgentAction = new QAction(tr("No active agent"), this);
    noAgentAction->setDisabled(true);

    noInternetAction = new QAction(tr("Agent can't connect to the internet"), this);
    noInternetAction->setDisabled(true);

    resumePauseSyncAction = new QAction(tr("Pause sync"), this);
    connect(resumePauseSyncAction, SIGNAL(triggered()), this, SIGNAL(pauseSync()));

    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SIGNAL(about()));

    // To be only enabled during debug
    //shareAction = new QAction(tr("Share"), this);
    //connect(shareAction, SIGNAL(triggered()), this, SIGNAL(share(QString, QString, QString)));
    //shareAction->setDisabled(true);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SIGNAL(quit()));
}

void CustomTrayIcon::openSingleJobLocal(){
    QDesktopServices::openUrl(this->singleJob->getJob()->getLocal());
}

void CustomTrayIcon::openSingleJobRemote(){
    QDesktopServices::openUrl(this->singleJob->getJob()->getRemote());
}

void CustomTrayIcon::launchAgent(){
    emit launchAgentSignal();
}

void CustomTrayIcon::workOccuring(){
    if(!animationOn){
        animationTimer->start();
        animationOn = true;
    }
}

void CustomTrayIcon::workDone(){
    if(animationOn){
        animationTimer->stop();
        animationOn = false;
        this->setIconNormal();
        normalIcon = true;
    }
}

void CustomTrayIcon::changeIcon(){
    if(normalIcon){
        this->setIconBusy();
        //this->animationTimer->setInterval(700);
        normalIcon = false;
    }else{
        this->setIconNormal();
        //this->animationTimer->setInterval(800);
        normalIcon = true;
    }
}

void CustomTrayIcon::setIconNormal(){
    qDebug() << "SETTING ICON normal";
    this->setIcon(this->iconActive);
}

void CustomTrayIcon::setIconBusy(){
    qDebug() << "SETTING ICON busy";
    this->setIcon(this->iconTransfer);
}

void CustomTrayIcon::setIconInactive(){
    qDebug() << "SETTING ICON inactive";
    this->setIcon(this->iconInactive);
}


bool CustomTrayIcon::agentUp(){
    return this->syncAgentUp;
}

void CustomTrayIcon::debug(QString s){
    if(this->debugMode)
        qDebug()<<"  -- JOBSMANAGER --   :    "<<s;
}
