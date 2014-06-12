#ifndef JSEVENTHANDLER_H
#define JSEVENTHANDLER_H

#include <QObject>
#include <QFileDialog>
#include <QDesktopServices>

class JSEventHandler : public QObject
{
    Q_OBJECT
public:
    explicit JSEventHandler(QObject *parent = 0);

public slots:
    QString getPath();
    void openUrl(QUrl);

signals:
    void jsEvent();
};

#endif // JSEVENTHANDLER_H
