#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent, int width, int height) :
    QDialog(parent)
{
    aboutWebView = new QWebView(this);
    aboutWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    this->resize(width, height);
    this->aboutWebView->resize(this->size());
}

void AboutDialog::show(){
    aboutWebView->load(QUrl("qrc:/webkit-sources/about.html"));
    connect(aboutWebView, SIGNAL(linkClicked(QUrl)), this, SLOT(openLink(QUrl)) );
    this->showNormal();
    qDebug()<<QString::number(this->size().height())<<QString::number(this->size().width());
    qDebug()<<QString::number(aboutWebView->size().height())<<QString::number(aboutWebView->size().width());
}

void AboutDialog::openLink(QUrl link){
    QDesktopServices::openUrl(link);
}
