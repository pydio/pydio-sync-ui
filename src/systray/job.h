#ifndef JOB_H
#define JOB_H

#include <QObject>

class Job : public QObject
{
    Q_OBJECT
public:
    explicit Job(QObject *parent = 0);
    Job(QString, QString, bool, double);
    void update(QString, bool, double);
    QString getJobDescription();
    QString getId();

signals:
    void updated(QString id, QString desc);

private:
    QString name;
    bool idle;
    bool status;
    double remainingTime;
    QString id;

};

#endif // JOB_H
