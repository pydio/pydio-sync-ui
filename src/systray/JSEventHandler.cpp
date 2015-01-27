#include "JSEventHandler.h"
#include <QDebug>

JSEventHandler::JSEventHandler(QObject *parent) :
    QObject(parent)
{
}

QString JSEventHandler::getPath()
{
   QString pydioDir = QDir::homePath() + "/pydio";
   return QFileDialog::getExistingDirectory(0, tr("Select local path"), pydioDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

void JSEventHandler::openUrl(QString toOpen)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(toOpen));
}

void JSEventHandler::openLogs(){
    QString logDir = QDir::homePath() + "/.pydio_data";
    qDebug()<<logDir;
    QDesktopServices::openUrl(QUrl::fromLocalFile(logDir));
}
