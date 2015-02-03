#ifndef GLOBALS_H
#define GLOBALS_H

const int WIDTH = 480;
const int HEIGHT = 700;
const int WIDTH_OFFSET_WIN = 15;
const int HEIGHT_OFFSET_WIN = 80;


const int POLL_TIME_AFTER_404 = 5000;
const int POLL_INTERVAL = 2000;
const static int MAX_CONNECTION_ATTEMPTS = 2;


const QString AGENT_SERVER_URL = "http://127.0.0.1:";
const static int DEFAULT_PORT = 5556;
const QString AGENT_FILE_NAME_WIN = "/pydio-agent.exe";
const QString ABOUT_PAGE_PATH = "/res/index.html#/about";
const QString PORT_CONFIG_FILE_NAME = "ports_config";
const QString PYDIO_DATA_DIR = "/Pydio";

const QString PYDIO_VERSION = "##VERSION##";
const QString COMMIT_ID = "##COMMIT##";
const QString VERSION_DATE = "##VERSION_DATE##";
const QString BUILD_CHANNEL = "##BUILD_CHANNEL##";

#endif // GLOBALS_H
