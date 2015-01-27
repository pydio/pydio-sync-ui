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
#include <QWebView>
#include <QWebFrame>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyleFactory>
#include <QMessageBox>

#include <portconfigurer.h>
#include <JSEventHandler.h>
#include <httpmanager.h>
#include <jobaction.h>
#include <customtrayicon.h>
#include <cmdhelper.h>
#include <aboutdialog.h>

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
protected slots:
    void closeEvent(QCloseEvent *);

private slots:
    void show();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void cleanQuit();
    void agentReached();
    void createTrayIcon();
    void notFoundFromPython();
    void connectionLost();
    void about();

private:
    QWebView *settingsWebView;

    bool globalRunningStatus;

    CustomTrayIcon *tray;

    HTTPManager *httpManager;
    QTimer *pollTimer;

    const int WIDTH = 480;
    const int HEIGHT = 700;
    const int WIDTH_OFFSET_WIN = 15;
    const int HEIGHT_OFFSET_WIN = 80;
    const int POLL_TIME_AFTER_404 = 5000;
    const int POLL_INTERVAL = 2000;
    const QString AGENT_SERVER_URL = "http://127.0.0.1:";
    const QString AGENT_FILE_NAME_WIN = "/pydio-agent.exe";
    const QString ABOUT_PAGE_PATH = "/res/index.html#/about";

    PortConfigurer *portConfigurer;
    JSEventHandler *jsDialog;
    CmdHelper *cmdHelper;

    AboutDialog *aboutDialog;

    QString pathToWinAgent;
};

#endif // QT_NO_SYSTEMTRAYICON

#endif
