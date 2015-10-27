#include "sharefile.h"
#include <QWebElement>

ShareFile::ShareFile(QWidget *parent) :
    QDialog(parent)
{
    shareWebView = new QWebView(this);
    shareWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
        this->resize(WIDTH, HEIGHT);
}

void ShareFile::show(){
    shareWebView->load(QUrl("qrc:/webkit-sources/share.html"));
    connect(shareWebView, SIGNAL(linkClicked(QUrl)), this, SLOT(openLink(QUrl)));
    this->showNormal();
    this->raise();
    this->activateWindow();
    shareWebView->resize(this->width(), shareWebView->height());
    this->adjustSize();
}

void ShareFile::openLink(QUrl link){
    QDesktopServices::openUrl(link);
}
