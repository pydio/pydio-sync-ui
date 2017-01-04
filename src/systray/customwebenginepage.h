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

        if(url.toString().indexOf("/app/index.html") != -1){
            qDebug() << "The requested window was loaded in the Qt managed webview";
         return true;
        }
        if (type == QWebEnginePage::NavigationTypeLinkClicked){
            // The Qt page requests the desktop to load the link in the default browser
            QDesktopServices::openUrl(url);
            return false;
        }
        return true;
    }
};

#endif // CUSTOMWEBENGINEPAGE_H
