#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    aboutWebView = new QWebView(this);
    aboutWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
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
