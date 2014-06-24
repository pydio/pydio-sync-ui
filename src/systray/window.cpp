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
        connect(t, SIGNAL(timeout()), this, SLOT(cleanQuit()));
        t->setInterval(5000);
        t->setSingleShot(true);
        t->start();
        qDebug()<<"Dumb test, will exit in 5 seconds...";
    }
    else{
        portConfigurer = new PortConfigurer(parser.value(pathArgument));
        createActions();
        createTrayIcon();
        trayIcon->show();

        jobActions = new QHash<QString, QAction*>();

        pollTimer = new QTimer(this);
        pollTimer->setInterval(2000);
        pollTimer->setSingleShot(true);

        poller = new HTTPPoller(this);
        connect(pollTimer, SIGNAL(timeout()), poller, SLOT(poll()));
        connect(poller, SIGNAL(requestFinished()), pollTimer, SLOT(start()));
        connect(poller, SIGNAL(newJob(QString, QString)), this, SLOT(onNewJob(QString, QString)));
        connect(poller, SIGNAL(jobUpdated(QString, QString)), this, SLOT(onJobUpdated(QString, QString)));
        connect(poller, SIGNAL(jobDeleted(QString)),this, SLOT(onJobDeleted(QString)));
        connect(poller, SIGNAL(connectionProblem()), this, SLOT(connectionProblem()));
        connect(poller, SIGNAL(agentReached()), this, SLOT(agentReached()));
        connect(poller, SIGNAL(noActiveJobsAtLaunch()), this, SLOT(show()));
        //connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(focusChanged(QWidget*, QWidget*)));

        jsDialog = new JSEventHandler(this);

        portConfigurer->updatePorts();
        poller->setUrl("http://127.0.0.1:" + portConfigurer->port("flask_api") + "/jobs");
        poller->poll();

        this->setWindowFlags(Qt::Tool);
        //Qt::FramelessWindowHint);
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

    QUrl syncUrl = QUrl("http://127.0.0.1:" + portConfigurer->port("flask_api"));
    settingsWebView->load(syncUrl);
    connect(settingsWebView->page(), SIGNAL(linkClicked(QUrl)), jsDialog, SLOT(openUrl(QUrl)));
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

void Window::focusChanged(QWidget* old, QWidget* newFocused){
    if(old == settingsWebView){
        this->close();
    }
}

void Window::closeEvent(QCloseEvent *e)
{
    settingsWebView->stop();
    settingsWebView->disconnect();
    this->close();
}

void Window::createActions()
{
    settingsAction = new QAction(tr("Configure.."), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(show()));
    settingsAction->setDisabled(true);

    noJobAction = new QAction(tr("No active job"), this);
    noJobAction->setDisabled(true);

    noAgentAction = new QAction(tr("No active agent"), this);
    noAgentAction->setDisabled(true);

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
    case QSystemTrayIcon::Trigger:
        break;
    default:
        ;
    }
}

void Window::cleanQuit()
{
    poller->disconnect();
    this->close();
    settingsWebView->clearFocus();
    this->clearFocus();
    emit qApp->quit();
}

void Window::onNewJob(QString id, QString desc)
{
    trayIconMenu->removeAction(noJobAction);

    QAction *newAction = new QAction(desc, this);
    newAction->setDisabled(true);
    jobActions->insert(id, newAction);
    trayIconMenu->insertAction(settingsAction, newAction);
}

void Window::onJobUpdated(QString id, QString desc)
{
    jobActions->operator [](id)->setText(desc);
}

void Window::onJobDeleted(QString id){
    trayIconMenu->removeAction(jobActions->operator [](id));
    jobActions->remove(id);
    if(jobActions->isEmpty())
    {
        trayIconMenu->insertAction(settingsAction, noJobAction);
    }
}

void Window::jobsCleared(){
    foreach(const QString &k, jobActions->keys()){
        qDebug()<<k<<"deleted";
        trayIconMenu->removeAction(jobActions->value(k));
    }
    jobActions->clear();
    trayIconMenu->removeAction(noJobAction);
    trayIconMenu->insertAction(settingsAction, noAgentAction);
}

void Window::agentReached(){
    qDebug()<<"agent reached";
    trayIconMenu->removeAction(noAgentAction);
    trayIconMenu->insertAction(settingsAction, noJobAction);
    settingsAction->setDisabled(false);
    portConfigurer->updatePorts();
    poller->setUrl("http://127.0.0.1:" + portConfigurer->port("flask_api") + "/jobs");
}

void Window::connectionProblem(){
    foreach(const QString &k, jobActions->keys()){
        trayIconMenu->removeAction(jobActions->value(k));
    }
    jobActions->clear();
    trayIconMenu->removeAction(noJobAction);
    trayIconMenu->insertAction(settingsAction, noAgentAction);
    settingsAction->setDisabled(true);
}

#endif
