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
#ifndef CUSTOMTRAYICON_H
#define CUSTOMTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QHash>
#include <jobmenu.h>
#include <jobaction.h>
#include <QDebug>
#include <QTimer>
#include <globals.h>


class CustomTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit CustomTrayIcon(QObject *parent = 0);
    bool agentUp();
signals:
    void launchAgentSignal();
    void pauseSync();
    void resumeSync();
    void about();
    void quit();
    void connectionLostSignal();
    void share(QString, QString, QString);

public slots:
    void connectionMade();
    void connectionLost();
    void jobsCleared(QString reason="UNKNOWN REASON");
    void onNewJob(Job *newJob);
    void onJobDeleted(QString id);
    void onJobUpdate(QString id);
    void openSingleJobLocal();
    void openSingleJobRemote();
    void launchAgent();
    void changeIcon();
    void noInternetConnection();
    void internetConnectionOk();
    void notificationReceived(QString jobId, QString type, QString message);

private:
    void createMainMenu();
    void createActions();
    void checkJobs();
    void insertNewJobMenu(Job*);
    void removeSingleJob();
    void insertSingleJob(Job *job);
    void debug(QString);
    void workOccuring();
    void workDone();
    void setIconNormal();
    void setIconBusy();
    void setIconInactive();

    QTimer *animationTimer;
    QMenu *mainMenu;
    QHash<QString, JobMenu*> *jobMenus;
    QAction *noAgentAction;
    QAction *noJobAction;
    QAction *noInternetAction;
    QAction *settingsAction;
    QAction *quitAction;
    QAction *reconnectAction;
    QAction *resumePauseSyncAction;
    QAction *aboutAction;
    jobAction *singleJob;
    QAction *singleJobLocal;
    QAction *singleJobRemote;
    QAction *separatorAction;
    //QAction *shareAction;

    bool globalRunningStatus;
    bool syncAgentUp;
    bool agentConnectedToInternet;
    bool working;
    bool animationOn;
    bool normalIcon;
    bool debugMode;
    QString pathToWinAgent;
};

#endif // CUSTOMTRAYICON_H
