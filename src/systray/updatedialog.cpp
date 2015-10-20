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
#include "updatedialog.h"

UpdateDialog::UpdateDialog(QWidget *parent) :
    QDialog(parent)
{
    //this->setFixedSize(450, 150);
    laterButton = new QPushButton(tr("Download later"), this);
    downloadButton = new QPushButton(tr("Download"), this);
    label = new QLabel(this);
    label->setTextFormat(Qt::RichText);
    connect(laterButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(downloadButton, SIGNAL(clicked()), this, SLOT(download()));

    layout = new QGridLayout(this);
}

void UpdateDialog::proposeDownload(QString package, QString version, QString releaseNote, QString md5){
    qDebug()<<package<<version<<releaseNote<<md5;
    url = package;
    label->setText("New Pydio available : v"+version);
    layout->addWidget(label, 0, 0);
    layout->addWidget(laterButton, 1, 0);
    layout->addWidget(downloadButton, 1, 1);
    this->setLayout(layout);
    this->exec();
}

void UpdateDialog::download(){
    QDesktopServices::openUrl(url);
}
