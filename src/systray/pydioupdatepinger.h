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
#ifndef PYDIOUPDATEPINGER_H
#define PYDIOUPDATEPINGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QUrl>

#include <globals.h>

class PydioUpdatePinger : public QObject
{
    Q_OBJECT
public:
    explicit PydioUpdatePinger(QObject *parent = 0);
    void lookForUpdate();

private:
    QNetworkAccessManager *manager;

signals:
    void updateFound(QString, QString, QString, QString);

public slots:
    void requestFinished(QNetworkReply*);
};

#endif // PYDIOUPDATEPINGER_H
