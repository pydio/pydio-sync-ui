#include "window.h"

#ifndef QT_NO_SYSTEMTRAYICON

Window::Window()
{
    QCommandLineParser parser;
    QCommandLineOption pathToWinAgentOption("p", "Path to sync agent", "agentPath");
    parser.addOption(pathToWinAgentOption);
    QCommandLineOption dumbTestOption("test", "Toggle dumb test.");
    parser.addOption(dumbTestOption);
    parser.process(*qApp);

    if(parser.isSet((pathToWinAgentOption))){
        this->pathToWinAgent = parser.value((pathToWinAgentOption));
    }
    else{
        this->pathToWinAgent = QDir::currentPath() + AGENT_FILE_NAME_WIN;
        qDebug()<<this->pathToWinAgent;
    }

    if(parser.isSet(dumbTestOption)){
        QTimer *t = new QTimer(this);
        connect(t, SIGNAL(timeout()), qApp, SLOT(quit()));
        t->setInterval(5000);
        t->setSingleShot(true);
        t->start();
        qDebug()<<"Dumb test, will exit in 5 seconds...";
    }
    else{
        cmdHelper = new CmdHelper(this, pathToWinAgent);
#ifdef Q_OS_WIN
        cmdHelper->launchAgentWin();
#endif
#ifdef Q_OS_MAC
        cmdHelper->launchAgentMac();
#endif

        portConfigurer = new PortConfigurer(QDir::homePath() + "/.pydio_data/ports_config");
        pollTimer = new QTimer(this);
        pollTimer->setInterval(POLL_INTERVAL);
        pollTimer->setSingleShot(true);

        httpManager = new HTTPManager(this);
        this->createTrayIcon();
        tray->show();

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

        connect(tray, SIGNAL(about()), this, SLOT(about()));
        connect(tray, SIGNAL(pauseSync()), httpManager, SLOT(pauseSync()));
        connect(tray, SIGNAL(resumeSync()), httpManager, SLOT(resumeSync()));
        connect(tray, SIGNAL(quit()), this, SLOT(cleanQuit()));
        connect(tray, SIGNAL(launchAgentSignal()), cmdHelper, SLOT(launchAgentWin()));
        connect(cmdHelper, SIGNAL(winAgentLaunched()), this, SLOT(show()));

        jsDialog = new JSEventHandler(this);

        portConfigurer->updatePorts();
        httpManager->setUrl(AGENT_SERVER_URL + portConfigurer->port());
        httpManager->poll();

        //this->setWindowFlags(Qt::Tool);
        setWindowTitle(tr("Pydio"));
    }
}

void Window::show()
{
    settingsWebView = new QWebView();
    settingsWebView->settings()->setAttribute( QWebSettings::JavascriptEnabled, true);
    settingsWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    settingsWebView->setContextMenuPolicy(Qt::NoContextMenu);
    this->setCentralWidget(settingsWebView);

#ifdef Q_OS_WIN
    settingsWebView->setStyle(QStyleFactory::create("windows"));
#endif

    QUrl syncUrl = QUrl(AGENT_SERVER_URL + portConfigurer->port());

    httpManager->testWebView();
    settingsWebView->load(syncUrl);

    // link the javascript dialog of the ui to the system FileDialog
    settingsWebView->page()->currentFrame()->addToJavaScriptWindowObject("PydioQtFileDialog", jsDialog);

    int desktopHeight = QApplication::desktop()->height();
    int desktopWidth = QApplication::desktop()->width();
    int tray_x = this->tray->geometry().center().x();

    if(desktopHeight < 800){
        this->resize(WIDTH, desktopHeight - 105);
    }
    else{
        this->resize(WIDTH, HEIGHT);
    }
    this->setFixedWidth(WIDTH);
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

    this->raise();
    this->showNormal();
}

void Window::closeEvent(QCloseEvent *e)
{
    settingsWebView->stop();
    settingsWebView->disconnect();
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
    QMessageBox  msgBox(this);
    msgBox.setWindowTitle("About Pydio UI");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("Pydio Desktop Sync<br>v0.9 alpha<br>Learn more :<a href=http://liftoff.pydio.com>http://liftoff.pydio.com</a>");
    msgBox.exec();
}

void Window::cleanQuit(){
#ifdef Q_OS_WIN
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
    httpManager->setUrl(AGENT_SERVER_URL + portConfigurer->port());
}

void Window::notFoundFromPython(){
#ifdef Q_OS_MAC
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
}

#endif
