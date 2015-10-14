#ifndef SHAREFILE
#define SHAREFILE

#include <QDialog>
#include <QWebView>
#include <QApplication>
#include <QDesktopServices>
#include <globals.h>

class ShareFile : public QDialog
{
    Q_OBJECT
public:
    explicit ShareFile(QWidget *parent = 0);
    void show();
    QWebView *shareWebView;

signals:

public slots:
    void openLink(QUrl);

};

#endif // SHAREFILE

