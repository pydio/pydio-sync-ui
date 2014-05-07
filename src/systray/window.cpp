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
#include "smoketest.h"


#ifndef QT_NO_SYSTEMTRAYICON

#include <QtGui>

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>


//! [0]
Window::Window()
{

    QCommandLineParser parser;
    QCommandLineOption smokeTestOption("s", "Toggle smoke test.");
    parser.addOption(smokeTestOption);
    QCommandLineOption pathArgument("f", "Path to config file.", "filePath");
    parser.addOption(pathArgument);
    parser.process(*qApp);
    portConfigurer = new PortConfigurer(parser.value(pathArgument));

    createActions();
    createTrayIcon();
    trayIcon->show();

    centralWidget = new PydioGui(this);

    context = nzmqt::createDefaultContext(this);
    context->start();

    if(parser.isSet(smokeTestOption))
    {
        SmokeTest* smokeTest = new SmokeTest(*context, "tcp://127.0.0.1:5513", this);
        connect(smokeTest, SIGNAL(testFinished()), qApp, SLOT(quit()));
        smokeTest->launch();
    }
    else
    {
        req = new nzmqt::Requester(*context, portConfigurer->address("watch_socket") , "sync", this);
        connect(req, SIGNAL(replyReceived(QString)), this, SLOT(updateStatus(QString)));
        req->start();

        commandHandler = new ToggleStatusRequester(*context, portConfigurer->address("command_socket"), this);
        connect(commandHandler, SIGNAL(replyReceived(QString)), this, SLOT(updateStatus(QString)));

        sub = new nzmqt::Subscriber(*context, portConfigurer->address("pub_socket"), "sync", this);
        connect(sub, SIGNAL(pingReceived(QList<QByteArray>)), SLOT(pingReceived(QList<QByteArray>)));
        sub->start();

        setWindowTitle(tr("Sync UI"));
        resize(400, 300);
    }
}


void Window::pingReceived(QList<QByteArray> message)
{
    for(int i=0; i<message.size(); ++i){
        QString str(message[i].constData());
        trayIcon->showMessage("Sync Message", str.replace("sync ", ""));
        lastEventsMenu->addEvent(str);
    }
}

void Window::updateStatus(QString order){
    if(order == "RUNNING" && this->running == false)
    {
        trayIcon->showMessage("Status Message", "Synchronization started");
        this->running = true;
        lastEventsMenu->addEvent("Synchronization started");
        startAction->setText(tr("&Pause"));
    }
    else if(order == "PAUSED" && this->running == true)
    {
        trayIcon->showMessage("Status Message", "Synchronization paused");
        this->running = false;
        lastEventsMenu->addEvent("Synchronization paused");
        startAction->setText(tr("&Resume"));
    }
}

void Window::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    QDialog::setVisible(visible);
}

void Window::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        trayIcon->showMessage(tr("Info"),tr("The program will still be running right here"));
        hide();
        event->ignore();
    }
}

void Window::toggleJobStatus()
{
    commandHandler->sendRequest(running);
}

void Window::createActions()
{
    minimizeAction = new QAction(tr("&Minimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    startAction = new QAction(tr("&Start"), this);
    connect(startAction, SIGNAL(triggered()), this, SLOT(toggleJobStatus()));

    settingsAction = new QAction("Configure...", this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(cleanQuit()));
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    this->createLastEventsMenu();

    trayIconMenu->addMenu(lastEventsMenu);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(startAction);
    trayIconMenu->addAction(settingsAction);

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
    sub->closeSocket();
    req->closeSocket();
    commandHandler->closeSocket();
    emit qApp->quit();
}

#endif
