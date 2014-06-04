#ifndef PYDIOGUI_H
#define PYDIOGUI_H

#include <QWidget>
#include <QWebView>

namespace Ui {
class PydioGui;
}

class PydioGui : public QWidget
{
    Q_OBJECT

public:
    explicit PydioGui(QWidget *parent = 0);
    void load(QString);
    ~PydioGui();

private:
    Ui::PydioGui *ui;
    QWebView* view;
};

#endif // PYDIOGUI_H
