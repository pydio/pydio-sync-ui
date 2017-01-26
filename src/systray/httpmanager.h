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
#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <job.h>
#include <QHash>
#include <globals.h>
#include <QTimer>


class HTTPManager : public QObject
{
    Q_OBJECT
public:
     HTTPManager(QObject *parent = 0, QNetworkAccessManager* QNetworkAccessManager=NULL);
    void setUrl(QString, QString, QString);
    void testWebView();
    QString getAgentUsername();
    QString getAgentPassword();
    int wantIcon = 0;

signals:
   void requestFinished();
   void newJob(Job*);
   void jobUpdated(QString);
   void jobDeleted(QString);
   void connectionProblem();
   void agentReached();
   void noActiveJobsAtLaunch();
   void jobsCleared();
   void webUI404();
   void noInternetConnection();
   void internetConnectionOk();
   void jobNotifyMessage(QString,QString,QString);

public slots:
    void poll();
    void pollingFinished(QNetworkReply*);
    void headRequestFinished(QNetworkReply*);
    void resumeSync();
    void pauseSync();
    void terminateAgent();
    void provideAuthentication(QNetworkReply *, QAuthenticator *);

private:
    bool debugMode;
    QNetworkAccessManager *manager;
    QString serverUrl;
    QString serverUsername;
    QString serverPassword;
    QHash<QString, Job*> *jobs;
    int failed_attempts;
    bool initialConnection = true;
    bool launch;
    void checkNoJobAtLaunch();
    void debug(QString);
};

#endif // HTTPManager_H
