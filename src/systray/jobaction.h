#ifndef JOBACTION_H
#define JOBACTION_H

#include <QAction>
#include <job.h>

class jobAction : public QAction
{
    Q_OBJECT
public:
    explicit jobAction(QObject *parent = 0, Job *job = 0);
    Job* getJob();
    void setJob(Job* newJob);
    void update();

private:
    Job *job;

};

#endif // JOBACTION_H
