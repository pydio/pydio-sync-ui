#include "updatedownloader.h"

UpdateDownloader::UpdateDownloader(QObject *parent) :
    QObject(parent)
{
    //notifier = new QDialog(parent);
}

void UpdateDownloader::proposeDownload(QString package, QString version, QString releaseNote, QString md5){
    /*qDebug()<<package<<version<<releaseNote<<md5;
    notifier->setWindowFlags(Qt::Widget | Qt::CustomizeWindowHint);
    notifier->setFixedSize(400,250);

    QPushButton *laterButton = new QPushButton(tr("Download later"), notifier);

    QPushButton *downloadButton = new QPushButton(tr("Download"), notifier);

    //connect(normalmode, SIGNAL(clicked()), this, SLOT(fermer_normal()));
    //connect(pannecourant, SIGNAL(clicked()), this, SLOT(fermer_recup()));

    QGridLayout *layout = new QGridLayout(notifier);
    layout->addWidget(laterButton, 0, 0);
    layout->addWidget(downloadButton, 0, 1);
    setLayout(layout);

    notifier.exec();
    hide();
    this->show();*/
}

void UpdateDownloader::download(QUrl packageUrl){
    manager->get(QNetworkRequest(QUrl(packageUrl)));
}
