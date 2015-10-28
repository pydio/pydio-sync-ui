#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QMainWindow>
#include <QObject>
#include <globals.h>
#include <JSEventHandler.h>

QT_BEGIN_NAMESPACE
class QLocalServer;
QT_END_NAMESPACE

class LocalServer : public QObject
{
    Q_OBJECT
    //static QString fileName;

public:
    LocalServer(QObject *parent = 0);
    QString fileName;


signals:
    void OnFileNameChanged(QList<QString>);

private slots:
    void GenerateLink();
    QString getFileName();

private:
    QLocalServer *localserver;
    JSEventHandler *jseventhandler;
};

#endif // LOCALSERVER_H
