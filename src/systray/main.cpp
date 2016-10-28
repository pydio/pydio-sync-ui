/****************************************************************************
**
** Copyright (C) 2007-2015 Charles du Jeu - Abstrium SAS <team (at) pyd.io>
** This file is part of Pydio.
**
** Pydio is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  Pydio is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Pydio.  If not, see <http://www.gnu.org/licenses/>.
**
**  The latest code can be found at <http://pyd.io/>.
**
****************************************************************************/

#include <QApplication>
#include <QNetworkAccessManager>
#include <QtCore/QTranslator>
#ifndef QT_NO_SYSTEMTRAYICON

#include <QMessageBox>
#include "window.h"
int pydiosync(QApplication *app, QNetworkAccessManager *networkManager){
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QTranslator myappTranslator;
    myappTranslator.load(":/languages/pydioUI_" + QLocale::system().name());
    app->installTranslator(&myappTranslator);
    QApplication::setQuitOnLastWindowClosed(false);

    Window window(networkManager);
    return app->exec();
}

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(systray);
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "23654");
    qDebug() << "Yala REMOVE ME FOR PROD";
    // On some versions of Qt initialising the network manager before the app reduces CPU peaks
    QApplication *app = new QApplication(argc, argv);
    QNetworkAccessManager *networkManager = new QNetworkAccessManager; // Unfortunately iniatilizing it before QApplication causes missing graphics libraries errors on most platforms.
    return pydiosync(app, networkManager);
}

#else

#include <QLabel>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString text("QSystemTrayIcon is not supported on this platform");

    QLabel *label = new QLabel(text);
    label->setWordWrap(true);

    label->show();
    qDebug() << text;

    app.exec();
}
#endif
