#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QWebView>
#include <QApplication>
#include <QDesktopServices>
#include <globals.h>

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = 0);
    void show();
    QWebView *aboutWebView;

signals:

public slots:
    void openLink(QUrl);

};

#endif // ABOUTDIALOG_H
