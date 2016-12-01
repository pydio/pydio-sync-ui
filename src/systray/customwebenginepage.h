#ifndef CUSTOMWEBENGINEPAGE_H
#define CUSTOMWEBENGINEPAGE_H
#include <QObject>
#include <QWebEnginePage>
#include <QDesktopServices>

class CustomQWebEnginePage : public QWebEnginePage {
    Q_OBJECT

public:
    CustomQWebEnginePage(QObject* parent = 0) : QWebEnginePage(parent){}

    bool acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool isMainFrame){
         qDebug() << "acceptNavigationRequest " << url << " " << type << " " << isMainFrame;

        if (type == QWebEnginePage::NavigationTypeLinkClicked){
            QDesktopServices::openUrl(url);
            return false;
        }
        return true;
    }
};

#endif // CUSTOMWEBENGINEPAGE_H
