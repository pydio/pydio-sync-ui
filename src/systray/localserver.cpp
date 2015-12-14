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
    connect(localserver, SIGNAL(newConnection()), this->parent(), SLOT(share()));
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

    // read the data - file path
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::ReadOnly);
    buffer.append(clientConnection->readAll());
    QString temp(buffer);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(temp.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    QList<QString> ShareContent;
    ShareContent << jsonObject["JobId"].toString() << jsonObject["RelativePath"].toString() << jsonObject["FolderFlag"].toString();
    emit OnFileNameChanged(ShareContent);

    clientConnection->disconnectFromServer();
}

QString LocalServer::getFileName()
{
   QString path = LocalServer::fileName;
   return path;
}
