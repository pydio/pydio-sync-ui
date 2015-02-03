#ifndef PYDIOUPDATEPINGER_H
#define PYDIOUPDATEPINGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QUrl>

#include <globals.h>

class PydioUpdatePinger : public QObject
{
    Q_OBJECT
public:
    explicit PydioUpdatePinger(QObject *parent = 0);
    void lookForUpdate();

private:
    QNetworkAccessManager *manager;

signals:
    void updateFound(QString, QString, QString, QString);

public slots:
    void requestFinished(QNetworkReply*);
};

#endif // PYDIOUPDATEPINGER_H
