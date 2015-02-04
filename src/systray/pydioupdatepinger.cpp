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
#include "pydioupdatepinger.h"

PydioUpdatePinger::PydioUpdatePinger(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(parent);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
}

void PydioUpdatePinger::lookForUpdate(){    
    if(BUILD_CHANNEL == "##BUILD_CHANNEL##") {
        // We are on develop mode, let's do nothing
        return;
    }
    QString url = UPDATE_URL+"?package=pydio-sync"
                             "&channel="+BUILD_CHANNEL+"&version="+PYDIO_VERSION+"&"
                             "arch="+PYDIO_BUILD_ARCH+"&version_date="+VERSION_DATE;
    //qDebug()<<url;
    manager->get(QNetworkRequest(QUrl(url)));
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
