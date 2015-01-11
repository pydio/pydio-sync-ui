#ifndef CUSTOMTRAYICON_H
#define CUSTOMTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QHash>
#include <jobmenu.h>
#include <jobaction.h>
#include <QDebug>
#include <QTimer>

class CustomTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit CustomTrayIcon(QObject *parent = 0);

signals:
    void launchAgentSignal();
    void pauseSync();
    void resumeSync();
    void about();
    void quit();

public slots:
    void connectionMade();
    void connectionLost();
    void jobsCleared(QString reason="UNKNOWN REASON");
    void onNewJob(Job *newJob);
    void onJobDeleted(QString id);
    void onJobUpdate(QString id);
    void openSingleJobLocal();
    void openSingleJobRemote();
    void launchAgent();
    void changeIcon();
    void noInternetConnection();
    void internetConnectionOk();

private:
    void createMainMenu();
    void createActions();
    void checkJobs();
    void insertNewJobMenu(Job*);
    void removeSingleJob();
    void insertSingleJob(Job *job);
    void debug(QString);
    void workOccuring();
    void workDone();

    QTimer *animationTimer;
    QMenu *mainMenu;
    QHash<QString, JobMenu*> *jobMenus;
    QAction *noAgentAction;
    QAction *noJobAction;
    QAction *noInternetAction;
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
    bool agentConnectedToInternet;
    bool working;
    bool animationOn;
    bool normalIcon;
    bool debugMode;
    QString pathToWinAgent;
};

#endif // CUSTOMTRAYICON_H
