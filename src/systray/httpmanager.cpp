#include "HTTPManager.h"

HTTPManager::HTTPManager(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(parent);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(pollingFinished(QNetworkReply*)));
    serverUrl = "";
    jobs = new QHash<QString, Job*>();
    failed_attempts = -1;
    launch = true;
}

void HTTPManager::setUrl(QString servUrl)
{
    qDebug()<<"Server is"<<servUrl;
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
        if(strReply != "\"success\""){
            if(failed_attempts != 0){
                emit agentReached();
                failed_attempts = 0;
            }

            QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
            QJsonArray jsonJobs = jsonResponse.array();
            if(jsonJobs.count()==0){
                if(launch){
                    launch = false;
                    emit noActiveJobsAtLaunch();
                }
                if(!jobs->empty()){
                    this->jobs->clear();
                    emit jobsCleared();
                }
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
                    bool running = job["running"].toBool();
                    int queue_length = job["state"].toObject().operator []("global").toObject().operator[]("queue_length").toInt();
                    double eta = -1;
                    // if job has pending tasks we update it
                    if(queue_length > 0){
                        eta = job["state"].toObject().operator []("global").toObject().operator[]("eta").toDouble();
                    }

                    // if jobs doesn't exist here, we create it
                    if(!this->jobs->contains(jobId)){
                        if(job["active"].toBool()){
                            // create a new active job, add it and notify the main class
                            Job *newJob = new Job(jobId, label, running, eta, lastEventMessage);
                            connect(newJob, SIGNAL(updated(QString, QString)), this, SIGNAL(jobUpdated(QString, QString)));
                            this->jobs->insert(jobId, newJob);
                            emit this->newJob(newJob->getId(), newJob->getJobDescription());
                        }
                    }
                    // if job exists and is active, we update it, otherwise we delete it
                    else{
                        if(!job["active"].toBool()){
                            this->jobs->remove(jobId);
                            emit jobDeleted(jobId);
                        }
                        else{
                            jobs->value(jobId)->update(label, running, eta, lastEventMessage);
                        }
                    }
                }
                if(jobs->empty() && launch){
                    launch = false;
                    emit noActiveJobsAtLaunch();
                }
            }
        }
    }
    else{
        qDebug() << "HTTP Poller Reply failure :" <<reply->errorString();
        ++failed_attempts;
        if(failed_attempts == MAX_CONNECTION_ATTEMPTS){
            emit connectionProblem();
        }
    }
    reply->deleteLater();
    emit requestFinished();
}

void HTTPManager::start_all(){
    manager->get(QNetworkRequest(QUrl(this->serverUrl + "/cmd/start-all")));
}

void HTTPManager::pause_all(){
    manager->get(QNetworkRequest(QUrl(this->serverUrl + "/cmd/pause-all")));
}

void HTTPManager::terminateAgent(){
    manager->get(QNetworkRequest(QUrl(this->serverUrl + "/cmd/quit")));
}
