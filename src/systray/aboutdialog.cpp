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
#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    aboutWebView = new QWebEngineView(this);
    //--aboutWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    this->resize(WIDTH, HEIGHT);
}

void AboutDialog::show(){
    aboutWebView->load(QUrl("qrc:/webkit-sources/about.html"));
    connect(aboutWebView, SIGNAL(linkClicked(QUrl)), this, SLOT(openLink(QUrl)));
    this->showNormal();
    aboutWebView->resize(this->width(), aboutWebView->height());
    this->adjustSize();
}

void AboutDialog::openLink(QUrl link){
    QDesktopServices::openUrl(link);
}
