#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QDesktopServices>

class UpdateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UpdateDialog(QWidget *parent = 0);

signals:

public slots:
    void download();
    void proposeDownload(QString package, QString version, QString releaseNote, QString md5);

private:
    QNetworkAccessManager *manager;
    QGridLayout *layout;
    QPushButton *laterButton;
    QPushButton *downloadButton;
    QLabel *label;

    QString url;

};

#endif // UPDATEDIALOG_H
