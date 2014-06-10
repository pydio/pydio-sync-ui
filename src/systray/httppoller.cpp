#include "httppoller.h"

HTTPPoller::HTTPPoller(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(parent);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(pollingFinished(QNetworkReply*)));
    serverUrl = "";
    jobs = new QHash<QString, Job*>();
}

void HTTPPoller::setUrl(QUrl &servUrl)
{
    this->serverUrl = servUrl;
}

void HTTPPoller::poll()
{
    manager->get(QNetworkRequest(QUrl("http://127.0.0.1:5556/jobs")));
}


void HTTPPoller::pollingFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QString strReply = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

        QJsonArray jsonJobs = jsonResponse.array();
        foreach(QJsonValue jsonJob, jsonJobs)
        {
            QJsonObject job = jsonJob.toObject();
            QString jobId = job["id"].toString();
            QString directory = job["directory"].toString();
            bool running = job["running"].toBool();
            int queue_length = job["state"].toObject().operator []("global").toObject().operator[]("queue_length").toInt();
            double eta = -1;
            if(queue_length > 0){
                eta = job["state"].toObject().operator []("global").toObject().operator[]("eta").toDouble();
            }
            if(!this->jobs->contains(jobId))
            {
                if(job["active"].toBool()){
                    Job *newJob = new Job(jobId, directory, running, eta);
                    connect(newJob, SIGNAL(updated(QString, QString)), this, SIGNAL(jobUpdated(QString, QString)));
                    this->jobs->insert(jobId, newJob);
                    emit this->newJob(newJob->getId(), newJob->getJobDescription());
                }
            }
            else{
                if(!job["active"].toBool()){
                    this->jobs->remove(jobId);
                    emit jobDeleted(jobId);
                }
                else
                {
                    jobs->value(jobId)->update(directory, running, eta);
                    jobs->value(jobId)->update(directory, running, eta);
                }
            }
        }

        reply->deleteLater();
    }
    else {
        qDebug() << "HTTP Poller Reply failure :" <<reply->errorString();
        delete reply;
    }
    emit requestFinished();
}
