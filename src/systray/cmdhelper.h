#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QTimer>
#include <QStandardPaths>
#include <QFileInfo>
#include <globals.h>

class CmdHelper : public QObject
{
    Q_OBJECT
public:
    explicit CmdHelper(QObject *parent = 0, QString path = "");
    static QString getAppDataDir(){
        #ifdef Q_OS_WIN
            QProcessEnvironment a = QProcessEnvironment::systemEnvironment();
            return a.value("APPDATA") + '\\' + PYDIO_DATA_DIR;
        //return QFileInfo(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).path() + PYDIO_DATA_DIR;
        #endif
        #ifdef Q_OS_MAC
            return QFileInfo(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).path() + PYDIO_DATA_DIR;
        #endif
    }

signals:
    void winAgentLaunched();

public slots:
    void launchAgentMac(bool stopBeforeLaunch = false);
    void launchAgentWin();
    void stopAgentMac();

private:
    QString pathToWinAgent;
};

#endif // CMDHELPER_H
