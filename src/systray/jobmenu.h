#ifndef JOBMENU_H
#define JOBMENU_H

#include <QMenu>
#include <QAction>
#include <job.h>
#include <QDesktopServices>
#include <QUrl>

class JobMenu : public QMenu
{
    Q_OBJECT
public:
    explicit JobMenu(QWidget *parent = 0, Job *job = 0);
    Job* getJob();
    void update();

private:
    QAction *status;
    QAction *openLocalAction;
    QAction *openRemoteAction;
    Job *job;

public slots:
    void openLocal();
    void openRemote();

};

#endif // JOBMENU_H
