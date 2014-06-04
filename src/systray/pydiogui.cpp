#include "pydiogui.h"
#include "ui_pydiogui.h"

PydioGui::PydioGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PydioGui)
{
    view = new QWebView();
    ui->setupUi(this);
}

void PydioGui::load(QString url)
{
    view->load(QUrl(url));
}

PydioGui::~PydioGui()
{
    delete ui;
}
