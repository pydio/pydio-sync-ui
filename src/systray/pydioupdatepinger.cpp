#include "pydioupdatepinger.h"

PydioUpdatePinger::PydioUpdatePinger(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(parent);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
}

void PydioUpdatePinger::lookForUpdate(){
    manager->get(QNetworkRequest(QUrl(UPDATE_URL)));
}

void PydioUpdatePinger::requestFinished(QNetworkReply* reply){
    if (reply->error() == QNetworkReply::NoError){
        //read the server response
        QString strReply = (QString)reply->readAll();

        // parse JSON response
        QJsonObject jsonResponse = QJsonDocument::fromJson(strReply.toUtf8()).object();
        QJsonValue jsonPackage = jsonResponse.value("package");
        if(jsonPackage.isUndefined()){
            qDebug()<<"NO NEW VERSION";
        }else{
            QString package = jsonPackage.toString();
            QString version = jsonResponse.value("version").toString();
            QString releaseNote = jsonResponse.value("release_note").toString();
            QString md5 = jsonResponse.value("md5").toString();
            //qDebug()<<package<<version<<releaseNote<<md5;
            this->updateFound(package, version, releaseNote, md5);
        }
    }
    else{
        qDebug("ERROR WITH UPDATE REQUEST");
    }
    reply->deleteLater();
}
