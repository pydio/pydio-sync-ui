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
#include "httpmanager.h"
#include <QAuthenticator>

HTTPManager::HTTPManager(QObject *parent) :
    QObject(parent)
{
    debugMode = true;
    manager = new QNetworkAccessManager(parent);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(pollingFinished(QNetworkReply*)));
    connect(manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
                SLOT(provideAuthentication(QNetworkReply*,QAuthenticator*)));
    serverUrl = "";
    this->serverUsername = "";
    jobs = new QHash<QString, Job*>();
    failed_attempts = -1;
    launch = true;
}

void HTTPManager::setUrl(QString servUrl, QString username, QString password)
{
    debug("Server set to " + servUrl);
    this->serverUrl = servUrl;
    this->serverUsername = username;
    this->serverPassword = password;
    failed_attempts = -1;
    jobs->clear();
}

void HTTPManager::poll()
{
    manager->get(QNetworkRequest(QUrl(this->serverUrl + "/jobs-status")));
}

void HTTPManager::provideAuthentication(QNetworkReply *reply, QAuthenticator *authenticator)
{
    //qDebug() << reply->readAll(); // this is just to see what we received
    if(this->serverUsername != ""){
        authenticator->setUser(this->serverUsername);
        authenticator->setPassword(this->serverPassword);
    }
}

void HTTPManager::pollingFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError){
        //read the server response
        QString strReply = (QString)reply->readAll();

        // this non-case is for when we terminate the python agent
        if(strReply != "\"success\""){
            if(failed_attempts != 0){
                debug("-----------------------CONNECTION MADE-----------------------");
                emit agentReached();
                failed_attempts = 0;
            }

            // parse JSON response

            QJsonObject jsonResponse = QJsonDocument::fromJson(strReply.toUtf8()).object();
            QJsonValue value = jsonResponse.value("jobs");
            QJsonArray jsonJobs = value.toArray();
            bool agentInternetStatus = jsonResponse.value("is_connected_to_internet").toBool();

            if(!agentInternetStatus){
                emit noInternetConnection();
            }else{
                emit internetConnectionOk();
            }
            // process response
            if(!jsonResponse.isEmpty()){
                if(jsonJobs.count()==0){
                    this->checkNoJobAtLaunch();
                    if(!jobs->empty()){
                        this->jobs->clear();
                    }
                    this->debug("JSON JOBS EMPTY");
                    emit this->jobsCleared();
                }
                else{
                    foreach(QString id, jobs->keys()){
                        bool present = false;
                        foreach(QJsonValue jsonJob, jsonJobs){
                            QJsonObject job = jsonJob.toObject();
                            QString jobId = job["id"].toString();
                            if(jobId == id){
                                present = true;
                            }
                        }
                        if(present == false){
                            this->jobs->remove(id);
                            emit jobDeleted(id);
                        }
                    }

                    foreach(QJsonValue jsonJob, jsonJobs){
                        // parse job objects
                        QJsonObject job = jsonJob.toObject();
                        QString jobId = job["id"].toString();
                        QString label = job["label"].toString();
                        QString lastEventMessage = job["last_event"].toObject().operator []("message").toString();
                        QString local = job["directory"].toString();
                        QString remote = job["server"].toString();
                        bool running = job["running"].toBool();
                        int total_tasks = job["state"].toObject().operator []("tasks").toObject().operator[]("total").toInt();
                        double eta = -1;
                        bool hasWork = false;
                        // if job has pending tasks we update it
                        if(total_tasks > 0){
                            eta = job["state"].toObject().operator []("global").toObject().operator[]("eta").toDouble();
                            hasWork = true;
                        }

                        // if jobs doesn't exist here, we create it
                        if(!this->jobs->contains(jobId)){
                            if(job["active"].toBool()){
                                // create a new active job, add it and notify the main class
                                Job *newJob = new Job(jobId, label, running, hasWork, eta, lastEventMessage, local, remote);
                                connect(newJob, SIGNAL(updated(QString)), this, SIGNAL(jobUpdated(QString)));
                                this->jobs->insert(jobId, newJob);
                                emit this->newJob(newJob);
                            }
                        }
                        // if job exists and is active, we update it, otherwise we delete it
                        else{
                            if(!job["active"].toBool()){
                                this->jobs->remove(jobId);
                                emit jobDeleted(jobId);
                            }
                            else{
                                jobs->value(jobId)->update(label, running, hasWork, eta, lastEventMessage);
                            }
                        }
                    }
                }
            }
        }
    }
    else if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404){
        emit webUI404();
    }
    else{
        if(failed_attempts < MAX_CONNECTION_ATTEMPTS)
            debug("HTTP Poller Reply : " + reply->errorString());
        ++failed_attempts;
        if(failed_attempts >= MAX_CONNECTION_ATTEMPTS){
            debug("-----------------------MAX CONNECTION ATTEMPTS REACHED, CLEARING JOBS-----------------------");
            this->jobs->clear();
            emit connectionProblem();
        }
    }
    reply->deleteLater();
    emit requestFinished();
}

void HTTPManager::checkNoJobAtLaunch(){
    if(launch){
        launch = false;
        QTimer *t = new QTimer(this);
        connect(t, SIGNAL(timeout()), this, SIGNAL(noActiveJobsAtLaunch()));
        t->setInterval(2000);
        t->setSingleShot(true);
        t->start();
    }
}

void HTTPManager::testWebView(){
    manager->get(QNetworkRequest(QUrl(this->serverUrl + "/res/index.html")));
}

void HTTPManager::resumeSync(){
    manager->get(QNetworkRequest(QUrl(this->serverUrl + "/cmd/start-all")));
}

void HTTPManager::pauseSync(){
    manager->get(QNetworkRequest(QUrl(this->serverUrl + "/cmd/pause-all")));
}

void HTTPManager::terminateAgent(){
    manager->get(QNetworkRequest(QUrl(this->serverUrl + "/cmd/exit")));
}

void HTTPManager::debug(QString s){
    if(this->debugMode)
        qDebug()<<"  -- HTTPMANAGER --   :    "<<s;
}
