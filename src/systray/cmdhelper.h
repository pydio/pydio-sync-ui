#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QTimer>

class CmdHelper : public QObject
{
    Q_OBJECT
public:
    explicit CmdHelper(QObject *parent = 0, QString path = "");

signals:
    void winAgentLaunched();

public slots:
    void launchAgentMac();
    void launchAgentWin();
    void stopAgentMac();

private:
    QString pathToWinAgent;
};

#endif // CMDHELPER_H
