#ifndef JOB_H
#define JOB_H

#include <QObject>
#include <QUrl>
#include <QDateTime>

class Job : public QObject
{
    Q_OBJECT
public:
    explicit Job(QObject *parent = 0);
    Job(QString, QString, bool, double, QString, QString, QString);
    void update(QString, bool, double, QString);
    QString getJobDescription();
    QString getId();
    bool getStatus();
    QString getName();
    QUrl getLocal();
    QUrl getRemote();

signals:
    void updated(QString id);

private:
    QString name;
    bool idle;
    bool status;
    double remainingTime;
    QString id;
    QString lastEventMessage;
    QUrl local;
    QUrl remote;

    QString remainingTimeToString();

};

#endif // JOB_H
