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

#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QtWebKit>
#include <queuemenu.h>
#include <portconfigurer.h>
#include <QWebView>
#include <JSEventHandler.h>
#include <httppoller.h>
#include <QWebFrame>
#include <QApplication>
#include <QDesktopWidget>
#include <QHash>
#include <QMessageBox>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window();
protected:
    void closeEvent(QCloseEvent *);

private slots:
    void show();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void about();
    void cleanQuit();
    void onNewJob(QString jobId, QString desc);
    void onJobUpdated(QString jobId, QString desc);
    void onJobDeleted(QString jobId);
    void connectionProblem();
    void agentReached();
    void jobsCleared();

private:
    void createActions();
    void createTrayIcon();
    void createLastEventsMenu();

    QWebView *settingsWebView;

    QAction *noAgentAction;
    QAction *noJobAction;
    QAction *settingsAction;
    QAction *quitAction;
    QAction *reconnectAction;
    QAction *quitAgentAction;
    QAction *startAction;
    QAction *pauseAction;
    QAction *aboutAction;
    QHash<QString, QAction*> *jobActions;

    QueueMenu *lastEventsMenu;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    HTTPPoller *poller;
    QTimer *pollTimer;


    PortConfigurer *portConfigurer;
    JSEventHandler *jsDialog;
};

#endif // QT_NO_SYSTEMTRAYICON

#endif
