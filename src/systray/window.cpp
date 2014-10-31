#include "window.h"

#ifndef QT_NO_SYSTEMTRAYICON

Window::Window()
{
    QCommandLineParser parser;
    QCommandLineOption pathToWinAgent("p", "Path to sync agent", "agentPath");
    parser.addOption(pathToWinAgent);
    QCommandLineOption dumbTestOption("test", "Toggle dumb test.");
    parser.addOption(dumbTestOption);
    QCommandLineOption pathArgument("f", "Path to config file.", "filePath");
    parser.addOption(pathArgument);
    parser.process(*qApp);

    if(parser.isSet((pathToWinAgent))){
        this->pathToWinAgent = parser.value((pathToWinAgent));
    }
    else{
        this->pathToWinAgent = "C:\\Program Files (x86)\\PydioSync\\bin\\pydio-sync-agent-win-latest.exe";
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
        portConfigurer = new PortConfigurer(parser.value(pathArgument));

        pollTimer = new QTimer(this);
        pollTimer->setInterval(2000);
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
        connect(tray, SIGNAL(pauseSync()), httpManager, SLOT(pauseSync()));
        connect(tray, SIGNAL(resumeSync()), httpManager, SLOT(resumeSync()));
        connect(tray, SIGNAL(quit()), this, SLOT(cleanQuit()));

        jsDialog = new JSEventHandler(this);

        portConfigurer->updatePorts();
        httpManager->setUrl("http://127.0.0.1:" + portConfigurer->port("flask_api"));
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

    QUrl syncUrl = QUrl("http://127.0.0.1:" + portConfigurer->port("flask_api"));
    settingsWebView->load(syncUrl);
    // allows to click link in the webview
    //connect(settingsWebView->page(), SIGNAL(linkClicked(QUrl)), jsDialog, SLOT(openUrlSlot(QUrl)));
    // link the javascript dialog of the ui to the system FileDialog
    settingsWebView->page()->currentFrame()->addToJavaScriptWindowObject("PydioQtFileDialog", jsDialog);

    if(QApplication::desktop()->height() < 800){
        this->resize(480, QApplication::desktop()->height() - 100);
    }
    else{
        this->resize(480, 730);
    }
    this->setFixedWidth(480);
    if(tray->geometry().y() < QApplication::desktop()->height()*0.5)
    {
        this->move(this->tray->geometry().center().x() - this->width()/2, tray->geometry().bottom());
    }
    else{
        this->move(this->tray->geometry().center().x() - this->width()/2, QApplication::desktop()->height() - 80 - this->height());
    }
    this->QWidget::show();
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
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText("Pydio Desktop Sync<br>v0.9 alpha<br>Learn more :<a href=http://liftoff.pydio.com>http://liftoff.pydio.com</a>");
    msgBox.exec();
}

// asks the user if python agent has to be stopped and quit
void Window::cleanQuit(){
#ifdef Q_OS_WIN
    httpManager->terminateAgent();
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), qApp, SLOT(quit()));
    t->setInterval(3000);
    t->setSingleShot(true);
    t->start();
#endif
#ifdef Q_OS_MAC
    QProcess process;
    QString processName = "launchctl";
    QStringList arguments = QStringList() << "remove"<< "PydioSync";
    process.start(processName, arguments);
    process.waitForStarted();
    process.waitForFinished();
    arguments = QStringList() << "remove"<< "PydioSyncUI";
    process.start(processName, arguments);
    process.waitForStarted();
    process.waitForFinished();
    emit qApp->quit();
#endif
}

void Window::agentReached(){
    tray->connectionMade();
    //portConfigurer->updatePorts();
    //httpManager->setUrl("http://127.0.0.1:" + portConfigurer->port("flask_api"));
}

#endif
