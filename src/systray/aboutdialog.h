#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QWebView>

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = 0);
    void show();

signals:

public slots:

private:
    QWebView *aboutWebView;


};

#endif // ABOUTDIALOG_H
