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
const QString AGENT_FILE_NAME_LINUX = "/../pydio-agent/pydio-agent";
const QString MAC_SERVICE_FILE_AGENT = "io.pyd.sync.launcher.plist";
const QString MAC_SERVICE_NAME_AGENT = "PydioSync";
const QString MAC_SERVICE_NAME_UI = "PydioSyncUI";
const QString ABOUT_PAGE_PATH = "/res/index.html#/about";
const QString SHARE_PAGE_PATH = "/res/index.html#/share";
const QString PORT_CONFIG_FILE_NAME = "ports_config";
const QString PYDIO_DATA_DIR = "Pydio";

const QString PYDIO_VERSION = "##VERSION##";
const QString VERSION_DATE = "##VERSION_DATE##";
const QString BUILD_CHANNEL = "##BUILD_CHANNEL##";

const QString PYDIO_BUILD_ARCH = "##BUILD_ARCH##";

const QString UPDATE_URL = "https://pyd.io/update/";
const bool CHECK_FOR_UPDATE = true;


#endif // GLOBALS_H
