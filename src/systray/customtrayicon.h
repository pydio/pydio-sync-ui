#ifndef CUSTOMTRAYICON_H
#define CUSTOMTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QHash>
#include <jobmenu.h>
#include <jobaction.h>
#include <QDebug>

class CustomTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit CustomTrayIcon(QObject *parent = 0);

signals:
    void pauseSync();
    void resumeSync();
    void about();
    void quit();

public slots:
    void connectionMade();
    void connectionLost();
    void jobsCleared();
    void onNewJob(Job *newJob);
    void onJobDeleted(QString id);
    void onJobUpdate(QString id);
    void openSingleJobLocal();
    void openSingleJobRemote();
    void launchAgent();

private:
    void createMainMenu();
    void createActions();
    void checkJobs();
    void insertNewJobMenu(Job*);
    void removeSingleJob();
    void insertSingleJob(Job *job);
    void debug(QString);

    QMenu *mainMenu;
    QHash<QString, JobMenu*> *jobMenus;
    QAction *noAgentAction;
    QAction *noJobAction;
    QAction *settingsAction;
    QAction *quitAction;
    QAction *reconnectAction;
    QAction *resumePauseSyncAction;
    QAction *aboutAction;
    jobAction *singleJob;
    QAction *singleJobLocal;
    QAction *singleJobRemote;
    QAction *separatorAction;

    bool globalRunningStatus;
    bool syncAgentUp;
    bool debugMode;
};

#endif // CUSTOMTRAYICON_H
