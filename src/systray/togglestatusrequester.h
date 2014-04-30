#ifndef TOGGLESTATUSREQUESTER_H
#define TOGGLESTATUSREQUESTER_H

#include <nzmqt/nzmqt.hpp>
#include <QDebug>

class ToggleStatusRequester : public QObject
{
    Q_OBJECT
    typedef QObject super;

public:
    ToggleStatusRequester(nzmqt::ZMQContext& context, const QString& address_, QObject* parent);
    void sendRequest(bool);

signals:
    void replyReceived(QString reply);

private slots:
    void receiveReply(const QList<QByteArray>& reply);

private:
    QString address;
    QString requestMsg;

    nzmqt::ZMQSocket* socket;
};

#endif // TOGGLESTATUSREQUESTER_H
