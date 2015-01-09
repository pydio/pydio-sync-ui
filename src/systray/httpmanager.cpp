#include "httpmanager.h"

HTTPManager::HTTPManager(QObject *parent) :
    QObject(parent)
{
    debugMode = true;
    manager = new QNetworkAccessManager(parent);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(pollingFinished(QNetworkReply*)));
    serverUrl = "";
    jobs = new QHash<QString, Job*>();
    failed_attempts = -1;
    launch = true;
}

void HTTPManager::setUrl(QString servUrl)
{
    debug("Server set to " + servUrl);
    this->serverUrl = servUrl;
    failed_attempts = -1;
    jobs->clear();
}

void HTTPManager::poll()
{
    manager->get(QNetworkRequest(QUrl(this->serverUrl + "/jobs")));
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

            QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
            QJsonArray jsonJobs = jsonResponse.array();

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
        if(failed_attempts == MAX_CONNECTION_ATTEMPTS){
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
        emit noActiveJobsAtLaunch();
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
