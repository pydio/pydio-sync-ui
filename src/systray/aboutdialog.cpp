#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    aboutWebView = new QWebView(this);
}


void AboutDialog::show(){
    this->showNormal();
}
