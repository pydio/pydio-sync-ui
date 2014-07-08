#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <job.h>
#include <QHash>

class HTTPManager : public QObject
{
    Q_OBJECT
public:
    explicit HTTPManager(QObject *parent = 0);
    void setUrl(QString);

signals:
   void requestFinished();
   void newJob(QString, QString);
   void jobUpdated(QString, QString);
   void jobDeleted(QString);
   void connectionProblem();
   void agentReached();
   void noActiveJobsAtLaunch();
   void jobsCleared();

public slots:
    void poll();
    void pollingFinished(QNetworkReply*);
    void start_all();
    void pause_all();
    void terminateAgent();

private:
    QNetworkAccessManager *manager;
    QString serverUrl;
    QHash<QString, Job*> *jobs;
    const static int MAX_CONNECTION_ATTEMPTS = 2;
    int failed_attempts;
    bool launch;


};

#endif // HTTPManager_H
