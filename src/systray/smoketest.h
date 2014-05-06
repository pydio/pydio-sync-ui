#ifndef SMOKETEST_H
#define SMOKETEST_H

#include <nzmqt/nzmqt.hpp>
#include <QDebug>


class SmokeTest : public QObject
{
    Q_OBJECT
    typedef QObject super;

public:
    SmokeTest(nzmqt::ZMQContext& context, QString ad, QObject* parent);
    void sendRequest();

signals:
    void replyReceived(QString reply);
    void requestReceived(const QList<QByteArray> &request);

private slots:
    void receiveReply(const QList<QByteArray>& reply);
    void receiveRequest(const QList<QByteArray>& reply);

private:
    QString address;
    QString requestMsg;
    QString replyMsg;
    void endTest();

    nzmqt::ZMQSocket* reqSocket;
    nzmqt::ZMQSocket* repSocket;
};

#endif // SMOKETEST_H
