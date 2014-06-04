#ifndef HTTPPOLLER_H
#define HTTPPOLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class HTTPPoller : public QObject
{
    Q_OBJECT
public:
    explicit HTTPPoller(QObject *parent = 0);
    void setUrl(QUrl&);

signals:
   void requestFinished();

public slots:
    void poll();
    void pollingFinished(QNetworkReply*);

private:
    QNetworkAccessManager *manager;
    QUrl serverUrl;

};

#endif // HTTPPOLLER_H
