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
#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include <QCommandLineParser>
#include <QWebEngineView>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyleFactory>
#include <QMessageBox>
#include <QStandardPaths>

#include <portconfigurer.h>
#include <JSEventHandler.h>
#include <httpmanager.h>
#include <jobaction.h>
#include <customtrayicon.h>
#include <cmdhelper.h>
#include <aboutdialog.h>
#include <globals.h>
#include <pydioupdatepinger.h>
#include <updatedialog.h>
#include <localserver.h>
#include <QNetworkAccessManager>
#include <QWebChannel>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QDialog>
class PydioUiJS : public QObject{
    Q_OBJECT
    public slots:
        void jsTrigger(int x);
};

class JSEventHandler;

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QNetworkAccessManager*);
    PydioUiJS *pydiouijs;
    void doShow();

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
    void check_for_update();
    void share(QString JobId,QString FolderFlag,QString RelativePath);
    void openLink(QUrl);
    void doPoll();

private:
    QWebEngineView *settingsWebView;

    bool globalRunningStatus;

    CustomTrayIcon *tray;

    HTTPManager *httpManager;
    QTimer *pollTimer;

    PortConfigurer *portConfigurer;
    JSEventHandler *jsDialog;
    CmdHelper *cmdHelper;

    AboutDialog *aboutDialog;

    PydioUpdatePinger* updatePinger;
    UpdateDialog* updateDialog;

    QObject* localServer;
    void authenticate(const QUrl&, QAuthenticator*);
    QWebChannel* channel;
};

#endif // QT_NO_SYSTEMTRAYICON

#endif
