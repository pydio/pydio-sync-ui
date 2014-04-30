#include "queuemenu.h"

QueueMenu::QueueMenu(QString title, QWidget *parent) :
    QMenu(title, parent)
{
    lastEvents = new QQueue<QAction*>();
    this->addAction(new QAction("No recent events", this->parent()));
}

void QueueMenu::addEvent(QString eventString)
{
    QString date = QDateTime::currentDateTime().toString("hh:mm dd/MM");
    QAction *eventToAdd = new QAction( date + " - " + eventString, this->parent());
    eventToAdd->setDisabled(true);

    if(lastEvents->isEmpty())
    {
        this->clear();
        lastEvents->enqueue(eventToAdd);
        this->insertAction(lastEvents->last(), eventToAdd);
    }
    else{
        this->updateLastEvents();
        this->insertAction(lastEvents->last(), eventToAdd);
        lastEvents->enqueue(eventToAdd);
    }
}

void QueueMenu::updateLastEvents()
{
    if(lastEvents->size() > NB_MAX_UPDATES-1)
    {
        QAction *toDelete = lastEvents->dequeue();
        this->removeAction(toDelete);
        //delete toDelete;
    }
}
