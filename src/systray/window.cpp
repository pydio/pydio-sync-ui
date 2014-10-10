/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "window.h"

#ifndef QT_NO_SYSTEMTRAYICON

Window::Window()
{
    QCommandLineParser parser;
    QCommandLineOption dumbTestOption("test", "Toggle dumb test.");
    parser.addOption(dumbTestOption);
    QCommandLineOption pathArgument("f", "Path to config file.", "filePath");
    parser.addOption(pathArgument);
    parser.process(*qApp);

    if(parser.isSet(dumbTestOption)){
        QTimer *t = new QTimer(this);
        connect(t, SIGNAL(timeout()), qApp, SLOT(quit()));
        t->setInterval(5000);
        t->setSingleShot(true);
        t->start();
        qDebug()<<"Dumb test, will exit in 5 seconds...";
    }
    else{
        portConfigurer = new PortConfigurer(parser.value(pathArgument));

        jobActions = new QHash<QString, jobAction*>();
        globalRunningStatus = false;

        pollTimer = new QTimer(this);
        pollTimer->setInterval(2000);
        pollTimer->setSingleShot(true);

        httpManager = new HTTPManager(this);
        connect(pollTimer, SIGNAL(timeout()), httpManager, SLOT(poll()));
        connect(httpManager, SIGNAL(requestFinished()), pollTimer, SLOT(start()));
        connect(httpManager, SIGNAL(newJob(Job*)), this, SLOT(onNewJob(Job*)));
        connect(httpManager, SIGNAL(jobUpdated(QString)), this, SLOT(onJobUpdated(QString)));
        connect(httpManager, SIGNAL(jobDeleted(QString)),this, SLOT(onJobDeleted(QString)));
        connect(httpManager, SIGNAL(connectionProblem()), this, SLOT(connectionProblem()));
        connect(httpManager, SIGNAL(agentReached()), this, SLOT(agentReached()));
        connect(httpManager, SIGNAL(noActiveJobsAtLaunch()), this, SLOT(show()));
        connect(httpManager, SIGNAL(jobsCleared()), this, SLOT(jobsCleared()));

        createActions();
        createTrayIcon();
        trayIcon->show();

        jsDialog = new JSEventHandler(this);

        portConfigurer->updatePorts();
        httpManager->setUrl("http://127.0.0.1:" + portConfigurer->port("flask_api"));
        httpManager->poll();

        this->setWindowFlags(Qt::Tool);
        setWindowTitle(tr("Pydio"));
    }
}

void Window::show()
{
    settingsWebView = new QWebView();
    settingsWebView->settings()->setAttribute( QWebSettings::JavascriptEnabled, true);
    settingsWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    settingsWebView->setContextMenuPolicy(Qt::NoContextMenu);
    this->setCentralWidget(settingsWebView);

#ifdef Q_OS_WIN
    settingsWebView->setStyle(QStyleFactory::create("windows"));
#endif

    QUrl syncUrl = QUrl("http://127.0.0.1:" + portConfigurer->port("flask_api"));
    settingsWebView->load(syncUrl);
    // allows to click link in the webview
    connect(settingsWebView->page(), SIGNAL(linkClicked(QUrl)), jsDialog, SLOT(openUrl(QUrl)));
    // link the javascript dialog of the ui to the system FileDialog
    settingsWebView->page()->currentFrame()->addToJavaScriptWindowObject("PydioQtFileDialog", jsDialog);

    if(QApplication::desktop()->height() < 800){
        this->resize(480, QApplication::desktop()->height() - 100);
    }
    else{
        this->resize(480, 730);
    }
    this->setFixedWidth(480);
    if(trayIcon->geometry().y() < QApplication::desktop()->height()*0.5)
    {
        this->move(this->trayIcon->geometry().center().x() - this->width()/2, trayIcon->geometry().bottom());
    }
    else{
        this->move(this->trayIcon->geometry().center().x() - this->width()/2, QApplication::desktop()->height() - 80 - this->height());
    }
    this->QWidget::show();
}

void Window::closeEvent(QCloseEvent *e)
{
    settingsWebView->stop();
    settingsWebView->disconnect();
    settingsWebView->deleteLater();
    this->close();
}

void Window::createActions()
{
    settingsAction = new QAction(tr("Open Pydio"), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(show()));
    settingsAction->setDisabled(true);

    noJobAction = new QAction(tr("No active job"), this);
    noJobAction->setDisabled(true);

    noAgentAction = new QAction(tr("No active agent"), this);
    noAgentAction->setDisabled(true);

    resumePauseSyncAction = new QAction(tr("Pause sync"), this);
    connect(resumePauseSyncAction, SIGNAL(triggered()), httpManager, SLOT(pauseSync()));
    //quitAgentAction = new QAction(tr("Terminate agent"), this);
    //connect(quitAgentAction, SIGNAL(triggered()), httpManager, SLOT(terminateAgent()));

    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(cleanQuit()));
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    this->createLastEventsMenu();
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(settingsAction);
    trayIconMenu->insertAction(settingsAction, noAgentAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    const QIcon& icon = QIcon(":/images/Pydio16.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
}

void Window::createLastEventsMenu()
{
    lastEventsMenu = new QueueMenu("Last events", this);
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:;
        this->trayIcon->show();
        break;
    default:
        ;
    }
}

void Window::about(){
    QMessageBox  msgBox(this);
    msgBox.setWindowTitle("About Pydio UI");
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText("Pydio Desktop Sync<br>v0.9 alpha<br>Learn more :<a href=http://liftoff.pydio.com>http://liftoff.pydio.com</a>");
    msgBox.exec();
}

// asks the user if python agent has to be stopped and quit
void Window::cleanQuit(){
    /*QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Pydio UI", "Do you want to terminate the sync agent too ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        httpManager->terminateAgent();
    }*/
    emit qApp->quit();
}

void Window::onNewJob(Job* newJob){
    trayIconMenu->removeAction(noJobAction);
    jobAction *newAction = new jobAction(this, newJob);
    newAction->setDisabled(true);
    jobActions->insert(newJob->getId(), newAction);
    trayIconMenu->insertAction(settingsAction, newAction);
    this->checkAllJobsStatus();
}

void Window::onJobUpdated(QString id){
    jobActions->operator [](id)->update();
    qDebug()<<"on jobupdate";
    this->checkAllJobsStatus();
}

/* when a job is not active anymore:
 * remove corresponding object
 */
void Window::onJobDeleted(QString id){
    trayIconMenu->removeAction(jobActions->operator [](id));
    jobActions->remove(id);
    if(jobActions->isEmpty())
    {
        trayIconMenu->insertAction(settingsAction, noJobAction);
    }
    this->checkAllJobsStatus();
}

void Window::jobsCleared(){
    if(!jobActions->empty()){
        foreach(const QString &k, jobActions->keys()){
            qDebug()<<k<<"deleted";
            trayIconMenu->removeAction(jobActions->value(k));
        }
        jobActions->clear();
        qDebug()<<"jobs cleared";
    }
}

void Window::checkAllJobsStatus(){
    if(!jobActions->empty()){
        QList<jobAction*> jActions = jobActions->values();
        globalRunningStatus = jActions[0]->getJob()->getStatus();
        for(int i=0; i<jActions.size(); i++){
            Job *j = jActions[i]->getJob();
            globalRunningStatus = globalRunningStatus || j->getStatus();
        }
        if(globalRunningStatus){
            resumePauseSyncAction->setText(tr("Pause sync"));
            resumePauseSyncAction->disconnect();
            connect(resumePauseSyncAction, SIGNAL(triggered()), httpManager, SLOT(pauseSync()));
        }
        else{
            resumePauseSyncAction->setText(tr("Resume sync"));
            resumePauseSyncAction->disconnect();
            connect(resumePauseSyncAction, SIGNAL(triggered()), httpManager, SLOT(resumeSync()));
        }
    }
}


/* when python agent is reached :
 * clear the previous jobs in memory,
 * provides proper UI options
 */
void Window::agentReached(){
    qDebug()<<"agent reached";
    this->jobsCleared();
    trayIconMenu->removeAction(noAgentAction);
    trayIconMenu->insertAction(settingsAction, noJobAction);
    settingsAction->setDisabled(false);
    portConfigurer->updatePorts();
    httpManager->setUrl("http://127.0.0.1:" + portConfigurer->port("flask_api"));
    trayIconMenu->insertAction(aboutAction, resumePauseSyncAction);
    //trayIconMenu->insertAction(aboutAction, quitAgentAction);
}

/* when cannot reach the python agent :
 * clear jobs, provides proper UI options
 */
void Window::connectionProblem(){
    qDebug()<<"connection problem";
    this->jobsCleared();
    trayIconMenu->removeAction(noJobAction);
    trayIconMenu->insertAction(settingsAction, noAgentAction);
    settingsAction->setDisabled(true);
    trayIconMenu->removeAction(resumePauseSyncAction);
    //trayIconMenu->removeAction(quitAgentAction);
}


#endif
