#include "togglestatusrequester.h"

ToggleStatusRequester::ToggleStatusRequester(nzmqt::ZMQContext& context, const QString& address_, QObject* parent)
    : super(parent),address(address_)
{
    socket = context.createSocket(nzmqt::ZMQSocket::TYP_REQ, this);
    socket->setLinger(100);
    connect(socket, SIGNAL(messageReceived(const QList<QByteArray>&)), SLOT(receiveReply(const QList<QByteArray>&)));
    socket->connectTo(address);
    qDebug()<<"Connected to"<<address;
}

void ToggleStatusRequester::sendRequest(bool isRunning)
{
    socket->sendMessage(QString(isRunning?"PAUSE":"START").toLocal8Bit());
}

void ToggleStatusRequester::receiveReply(const QList<QByteArray>& reply)
{
    QString replyString;
    for(int i=0; i<reply.size(); ++i)
    {
        replyString += reply[i].constData();
    }
    emit replyReceived(replyString);
}

void ToggleStatusRequester::closeSocket()
{
    socket->close();
}
