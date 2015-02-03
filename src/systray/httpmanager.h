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
#include <globals.h>


class HTTPManager : public QObject
{
    Q_OBJECT
public:
    explicit HTTPManager(QObject *parent = 0);
    void setUrl(QString);
    void testWebView();

signals:
   void requestFinished();
   void newJob(Job*);
   void jobUpdated(QString);
   void jobDeleted(QString);
   void connectionProblem();
   void agentReached();
   void noActiveJobsAtLaunch();
   void jobsCleared();
   void webUI404();
   void noInternetConnection();
   void internetConnectionOk();

public slots:
    void poll();
    void pollingFinished(QNetworkReply*);
    void resumeSync();
    void pauseSync();
    void terminateAgent();

private:
    bool debugMode;
    QNetworkAccessManager *manager;
    QString serverUrl;
    QHash<QString, Job*> *jobs;
    int failed_attempts;
    bool launch;
    void checkNoJobAtLaunch();
    void debug(QString);


};

#endif // HTTPManager_H
