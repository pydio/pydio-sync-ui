#ifndef JSEVENTHANDLER_H
#define JSEVENTHANDLER_H

#include <QObject>
#include <QFileDialog>
#include <QDesktopServices>
#include <cmdhelper.h>

class JSEventHandler : public QObject
{
    Q_OBJECT
public:
    explicit JSEventHandler(QObject *parent = 0);

public slots:
    QString getPath();
    void openUrl(QString);
    void openLogs();
};

#endif // JSEVENTHANDLER_H
