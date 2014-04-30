#ifndef QUEUEMENU_H
#define QUEUEMENU_H

#include <QMenu>
#include <QQueue>
#include <QDebug>
#include <QDateTime>

class QueueMenu : public QMenu
{
    Q_OBJECT
public:
    explicit QueueMenu(QString title, QWidget *parent = 0);
    void addEvent(QString);
    void updateLastEvents();

private:
    const static  int NB_MAX_UPDATES = 10;
    QQueue<QAction*> *lastEvents;
};

#endif // QUEUEMENU_H
