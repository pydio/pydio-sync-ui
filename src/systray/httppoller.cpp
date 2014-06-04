#include "httppoller.h"

HTTPPoller::HTTPPoller(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(parent);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(pollingFinished(QNetworkReply*)));
    serverUrl = "";
}

void HTTPPoller::setUrl(QUrl &servUrl)
{
    this->serverUrl = servUrl;
}

void HTTPPoller::poll()
{
    qDebug()<<"poll";
    manager->get(QNetworkRequest(this->serverUrl));
}


void HTTPPoller::pollingFinished(QNetworkReply* reply)
{
    qDebug()<<"pollingFinished";
    qDebug()<<reply->readAll();
    reply->deleteLater();
    emit requestFinished();
}
