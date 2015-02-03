#ifndef UPDATEDOWNLOADER_H
#define UPDATEDOWNLOADER_H

#include <QObject>
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>


class UpdateDownloader : public QObject
{
    Q_OBJECT
public:
    explicit UpdateDownloader(QObject *parent = 0);

signals:

public slots:
    void download(QUrl packageUrl);
    void proposeDownload(QString package, QString version, QString releaseNote, QString md5);

private:
    QDialog* notifier;
    QNetworkAccessManager *manager;

};

#endif // UPDATEDOWNLOADER_H
