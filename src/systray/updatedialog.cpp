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
    //manager->get(QNetworkRequest(QUrl(url)));
    QDesktopServices::openUrl(url);
}
