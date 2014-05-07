#include "smoketest.h"

SmokeTest::SmokeTest(nzmqt::ZMQContext& context, QString ad, QObject* parent)
    : super(parent), address(ad)
{
    requestMsg = "smoketest";
    replyMsg = "all seems ok";

    reqSocket = context.createSocket(nzmqt::ZMQSocket::TYP_REQ, this);
    reqSocket->setLinger(100);
    connect(reqSocket, SIGNAL(messageReceived(const QList<QByteArray>&)), SLOT(receiveReply(const QList<QByteArray>&)));
    reqSocket->connectTo(address);

    repSocket = context.createSocket(nzmqt::ZMQSocket::TYP_REP, this);
    repSocket->setLinger(100);
    connect(repSocket, SIGNAL(messageReceived(const QList<QByteArray>&)), SLOT(receiveRequest(const QList<QByteArray>&)));
    repSocket->bindTo(address);
}

void SmokeTest::launch()
{
    reqSocket->sendMessage(requestMsg.toLocal8Bit());
    qDebug()<<"Smoke Test Client :: Request Sent >"<<requestMsg;
}

void SmokeTest::receiveReply(const QList<QByteArray>& reply)
{
    QString replyString;
    for(int i=0; i<reply.size(); ++i)
    {
        replyString += reply[i].constData();
    }
    qDebug()<<"Smoke Test Client :: Reply Received >"<<replyString;
    this->endTest();
}

void SmokeTest::receiveRequest(const QList<QByteArray>& request)
{
    QString requestString;
    for(int i=0; i<request.size(); ++i)
    {
        requestString += request[i].constData();
    }
    qDebug()<<"Smoke Test Server :: Request Received >"<<requestString;
    repSocket->sendMessage(replyMsg.toLocal8Bit());
    qDebug()<<"Smoke Test Server :: Reply Sent >"<<replyMsg;
}

void SmokeTest::endTest()
{
    repSocket->close();
    reqSocket->close();
    qDebug()<<"Smoke Test :: Done";
    emit testFinished();
}
