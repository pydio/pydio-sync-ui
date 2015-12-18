/****************************************************************************
**
** Copyright (C) 2015 Sagar Uday Kumar - Abstrium SAS <team (at) pyd.io>
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
#include "localserver.h"

#include <QtNetwork>
#include <stdlib.h>

#include <qlocalserver.h>
#include <qlocalsocket.h>


LocalServer::LocalServer(QObject *parent) :
    QObject(parent)
{
    localserver = new QLocalServer(this);
    QString namedpipe;
	#ifdef Q_OS_WIN
		namedpipe = LOCAL_SERVER_URL;
	#else
		namedpipe = "/tmp/" + LOCAL_SERVER_URL;
	#endif
	if (!localserver->listen(namedpipe)) {
		qDebug() << "Failed to open named pipe, deleting and trying again...";
		if (!QLocalServer::removeServer(namedpipe))
			qDebug() << "Failed to delete named pipe";
		if (!localserver->listen(namedpipe)) {
			qDebug() << "Trying to delete named pipe wasn't enough";
			//exit(-1);
		} else {
			qDebug() << "FIFO ok";
		}
		// return Unable to start the server msg
		//close();
		return;
	}
    connect(localserver, SIGNAL(newConnection()), this, SLOT(GenerateLink()));
}

void LocalServer::GenerateLink()
{
    QLocalSocket *clientConnection = localserver->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    if (!clientConnection->waitForReadyRead()) {
        clientConnection->disconnectFromServer();
        return;
    }

    // read the data recieved from the server and convert it into Json
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::ReadOnly);
    buffer.append(clientConnection->readAll());
    QString temp(buffer);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(temp.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    QString ItemType = "file";
    if (jsonObject["FolderFlag"].toString() == "true")
        ItemType = "folder";

    // emit the share signal
    emit share(jsonObject["JobId"].toString(), ItemType, jsonObject["RelativePath"].toString());

    clientConnection->disconnectFromServer();
}
