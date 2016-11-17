/****************************************************************************
**
** Copyright (C) 2007-2015 Charles du Jeu - Abstrium SAS <team (at) pyd.io>
** This file is part of Pydio.
**
** Pydio is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  Pydio is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Pydio.  If not, see <http://www.gnu.org/licenses/>.
**
**  The latest code can be found at <http://pyd.io/>.
**
****************************************************************************/
#include "window.h"
#include <QAuthenticator>

#ifdef Q_OS_WIN
    // seems required for msvc_2015
    #include "Windows.h"
    #pragma comment(lib,"user32.lib")
#endif

#ifndef QT_NO_SYSTEMTRAYICON

Window::Window(QNetworkAccessManager* manager)
{
    QCommandLineParser parser;
    QCommandLineOption pathToAgentOption("p", "Path to sync agent", "agentPath");
    parser.addOption(pathToAgentOption);
    QCommandLineOption skipAgentStartOption("s", "Do not try to start agent on start up", "skipAgentStart");
    parser.addOption(skipAgentStartOption);
    QCommandLineOption dumbTestOption("test", "Super simple start/stop test.");
    parser.addOption(dumbTestOption);

    parser.addHelpOption();
    //parser.addVersionOption();
    parser.process(*qApp);

    bool startAgent = true;
    if(parser.isSet(skipAgentStartOption) && parser.value(skipAgentStartOption) == "true"){
        startAgent = false;
    }
    cmdHelper = new CmdHelper(this, (parser.isSet(pathToAgentOption))?parser.value(pathToAgentOption):"");
    if(parser.isSet(dumbTestOption)){
        QTimer *t = new QTimer(this);
        connect(t, SIGNAL(timeout()), qApp, SLOT(quit()));
        t->setInterval(5000);
        t->setSingleShot(true);
        t->start();
        qDebug() << "Dumb test, will exit in 5 seconds...";
    }
    else{
        if(startAgent){
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
            cmdHelper->launchAgentProcess();
#elif defined(Q_OS_MAC)
            qDebug()<<"Starting agent via launchctl command.";
            cmdHelper->launchAgentMac();
#endif

        }

        QString dataDir = CmdHelper::getAppDataDir() +'/'+ PORT_CONFIG_FILE_NAME;
        portConfigurer = new PortConfigurer(dataDir);
        portConfigurer->updatePorts();

        localServer = new LocalServer(this);

        pollTimer = new QTimer(this);
        pollTimer->setInterval(POLL_INTERVAL);
        pollTimer->setSingleShot(true);

        httpManager = new HTTPManager(this, manager);
        this->createTrayIcon();
        tray->show();

        aboutDialog = new AboutDialog(this);

        connect(pollTimer, SIGNAL(timeout()), httpManager, SLOT(poll()));
        connect(httpManager, SIGNAL(requestFinished()), pollTimer, SLOT(start()));
        connect(httpManager, SIGNAL(newJob(Job*)), tray, SLOT(onNewJob(Job*)));
        connect(httpManager, SIGNAL(jobUpdated(QString)), tray, SLOT(onJobUpdate(QString)));
        connect(httpManager, SIGNAL(jobDeleted(QString)), tray, SLOT(onJobDeleted(QString)));
        connect(httpManager, SIGNAL(connectionProblem()), tray, SLOT(connectionLost()));
        connect(httpManager, SIGNAL(agentReached()), this, SLOT(agentReached()));
        connect(httpManager, SIGNAL(noActiveJobsAtLaunch()), this, SLOT(show()));
        connect(httpManager, SIGNAL(jobsCleared()), tray, SLOT(jobsCleared()));
        connect(httpManager, SIGNAL(webUI404()), this, SLOT(notFoundFromPython()));
        connect(httpManager, SIGNAL(noInternetConnection()), tray, SLOT(noInternetConnection()));
        connect(httpManager, SIGNAL(internetConnectionOk()), tray, SLOT(internetConnectionOk()));
        connect(httpManager, SIGNAL(connectionProblem()), this, SLOT(connectionLost()));
        connect(httpManager, SIGNAL(jobNotifyMessage(QString,QString,QString)), tray, SLOT(notificationReceived(QString,QString,QString)));

        connect(tray, SIGNAL(about()), this, SLOT(about()));
        connect(tray, SIGNAL(check_for_update()), this, SLOT(check_for_update()));
        connect(localServer, SIGNAL(share(QString,QString,QString)), this, SLOT(share(QString,QString,QString)));
        connect(tray, SIGNAL(pauseSync()), httpManager, SLOT(pauseSync()));
        connect(tray, SIGNAL(resumeSync()), httpManager, SLOT(resumeSync()));
        connect(tray, SIGNAL(quit()), this, SLOT(cleanQuit()));
        connect(tray, SIGNAL(launchAgentSignal()), cmdHelper, SLOT(launchAgentProcess()));

        settingsWebView = new QWebEngineView();

        jsDialog = new JSEventHandler(this);

        httpManager->setUrl(AGENT_SERVER_URL + portConfigurer->port(), portConfigurer->username(), portConfigurer->password());
        httpManager->poll();

        //this->setWindowFlags(Qt::Tool);
        setWindowTitle(PYDIO_DATA_DIR);
        setWindowIcon(QIcon(":/images/PydioSync-Systray-Mac.png"));
		
		#ifdef TARGET_OS_MAC
        if(QSysInfo::MacintoshVersion >= 12){
            qDebug() << "Starting MacOS extension";
            QObject *parent = 0;
            QProcess *macExtension = new QProcess(parent);
            macExtension->start("open", QStringList() << MAC_EXT);
            if(!macExtension->waitForFinished()){
                qDebug() << "Failed to launch MacOS extension.";
            } else {
                qDebug() << "Extension started.";
            }
            qDebug() << macExtension->readAll();
        }
        #endif


    }
}

void Window::authenticate(const QUrl &requestUrl, QAuthenticator *auth)
{
        auth->setUser(httpManager->getAgentUsername());
        auth->setPassword(httpManager->getAgentPassword());
}

void Window::show()
{
    settingsWebView = new QWebEngineView();
    channel = new QWebChannel(settingsWebView->page());
    settingsWebView->page()->setWebChannel(channel);
    pydiouijs = new PydioUiJS();

    channel->registerObject(QStringLiteral("pydiouijs"), pydiouijs);
    channel->registerObject(QStringLiteral("PydioQtFileDialog"), jsDialog);


    //--settingsWebView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    //--settingsWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    //--connect(settingsWebView, SIGNAL(linkClicked(QUrl)), this, SLOT(openLink(QUrl)) );
    // UNCOMMENT ME TODO
    settingsWebView->setContextMenuPolicy(Qt::NoContextMenu);
    this->setCentralWidget(settingsWebView);


#ifdef Q_OS_WIN
    settingsWebView->setStyle(QStyleFactory::create("windows"));
#endif

    QUrl syncUrl = QUrl(AGENT_SERVER_URL + portConfigurer->port());

    httpManager->testWebView();

    //--QNetworkAccessManager * nam = settingsWebView->page()->networkAccessManager();
//    nam->clearAccessCache();
    //--connect(nam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), httpManager,
        //--        SLOT(provideAuthentication(QNetworkReply*,QAuthenticator*)));

    settingsWebView->load(syncUrl);
    // link the javascript dialog of the ui to the system FileDialog
    //settingsWebView->page()->currentFrame()->addToJavaScriptWindowObject("PydioQtFileDialog", jsDialog);
    //settingsWebView->page()->runJavaScript("PydioQtFileDialog =" + jsDialog,  [](const QVariant &result){ qDebug() << result; });
    jsDialog->page = settingsWebView->page();
    qDebug() << QUrl(AGENT_SERVER_URL + portConfigurer->port() + ABOUT_PAGE_PATH);
    qDebug() << syncUrl;
    QObject::connect(settingsWebView->page(), &QWebEnginePage::authenticationRequired, this, &Window::authenticate);
    QRect rec = QApplication::desktop()->screenGeometry();
    int desktopHeight = rec.height();
    int desktopWidth = rec.width();
    int tray_x = this->tray->geometry().center().x();

    if(desktopHeight < 800){
        this->resize(WIDTH, desktopHeight - 105);
    }
    else{
        this->resize(WIDTH, HEIGHT);
    }
    //this->setFixedWidth(WIDTH);
    if(tray->geometry().y() < desktopHeight*0.5)
    {
        // MacOS case
        this->move(tray_x - this->width()/2, tray->geometry().bottom());
    }
    else{
        // Win case
        if(tray_x > (desktopWidth - this->width())/2){
            this->move(desktopWidth - this->width() - WIDTH_OFFSET_WIN, desktopHeight - HEIGHT_OFFSET_WIN - this->height());
        }
        else
            this->move(this->tray->geometry().center().x() - this->width()/2, desktopHeight - 80 - this->height());
    }
    settingsWebView->show();
    settingsWebView->raise();


    this->raise();
    this->showNormal();

}

void Window::check_for_update()
{
    if(CHECK_FOR_UPDATE){
        //qDebug() << "Check for update";
        updateDialog = new UpdateDialog(this);
        updatePinger = new PydioUpdatePinger(this);
        connect(updatePinger, SIGNAL(updateFound(QString,QString,QString,QString)),
                updateDialog, SLOT(proposeDownload(QString,QString,QString,QString)));
        updatePinger->lookForUpdate(AGENT_SERVER_URL + portConfigurer->port(), portConfigurer->username(), portConfigurer->password());
    }
}

void Window::closeEvent(QCloseEvent *e)
{
    settingsWebView->stop();
    settingsWebView->deleteLater();
    this->close();
}

void Window::createTrayIcon()
{
    tray = new CustomTrayIcon(this);
    setWindowIcon(tray->icon());
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
#ifdef Q_OS_WIN
        mouse_event(MOUSEEVENTF_RIGHTDOWN,0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
#endif
        break;
    default:
        ;
    }
}

void Window::about(){
    if(tray->agentUp()){
        this->show();
        settingsWebView->load(QUrl(AGENT_SERVER_URL + portConfigurer->port() + ABOUT_PAGE_PATH));
    }
    else{
        this->aboutDialog->show();
        //--aboutDialog->aboutWebView->page()->currentFrame()->addToJavaScriptWindowObject("PydioQtFileDialog", jsDialog);
    }
}

void Window::share(QString JobId,QString FolderFlag,QString RelativePath){
    if(tray->agentUp()){        
        this->show();
        this->activateWindow();
        settingsWebView->load(QUrl(AGENT_SERVER_URL + portConfigurer->port() + SHARE_PAGE_PATH + "/" + "standard" + "/" + JobId +"/"+ FolderFlag +"/"+  QUrl::toPercentEncoding(RelativePath)));
    }
}

void Window::cleanQuit(){
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    httpManager->terminateAgent();
    //wait for agent to terminate
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), qApp, SLOT(quit()));
    t->setInterval(1500);
    t->setSingleShot(true);
    t->start();
#endif
#ifdef Q_OS_MAC
    cmdHelper->stopAgentMac();
    emit qApp->quit();
#endif
}

void Window::agentReached(){
    tray->connectionMade();
}

void Window::connectionLost(){
    qDebug()<<"UPDATING PORTS FROM CONFIG FILE";
    portConfigurer->updatePorts();
    httpManager->setUrl(AGENT_SERVER_URL + portConfigurer->port(), portConfigurer->username(), portConfigurer->password());
}

void Window::notFoundFromPython(){
/*#ifdef Q_OS_MAC
    qDebug()<<"WILL LOAD PYTHON";
    settingsWebView->load(QUrl("qrc:/webkit-sources/reload_python.html"));
    cmdHelper->launchAgentMac(true);
    // will retry loading ui from agent in 5 seconds
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(show()));
    t->setInterval(POLL_TIME_AFTER_404);
    t->setSingleShot(true);
    t->start();
#endif
*/
}

void Window::openLink(QUrl link){
    QDesktopServices::openUrl(link);
}

void PydioUiJS::jsTrigger(int x){
    qDebug() << "O M G triggered from JS";
}

#endif
