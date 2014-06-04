#include "JSEventHandler.h"

JSEventHandler::JSEventHandler(QObject *parent) :
    QObject(parent)
{
}

QString JSEventHandler::getPath()
{
   return QFileDialog::getExistingDirectory(0, tr("Select local path"),"/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

void JSEventHandler::openUrl(QUrl toOpen)
{
    QDesktopServices::openUrl(QUrl(toOpen));
}

