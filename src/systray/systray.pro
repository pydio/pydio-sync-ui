TARGET = pydio-ui

DESTDIR = $$PWD/../../build
MOC_DIR = $${DESTDIR}/.moc
OBJECTS_DIR = $${DESTDIR}/.obj
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $$DESTDIR/.ui

HEADERS      += window.h \
                queuemenu.h \
                portconfigurer.h \
                JSEventHandler.h \
                job.h \
    httpmanager.h \
    jobaction.h \
    customtrayicon.h \
    jobmenu.h \
    cmdhelper.h
SOURCES       = main.cpp \
                window.cpp \
                queuemenu.cpp \
                portconfigurer.cpp \
                JSEventHandler.cpp \
                job.cpp \
    httpmanager.cpp \
    jobaction.cpp \
    customtrayicon.cpp \
    jobmenu.cpp \
    cmdhelper.cpp

RESOURCES     = systray.qrc

TRANSLATIONS += languages/pydioUI_fr_FR.ts

# install
 target.path = ./
 INSTALLS += target

QT += widgets \
      webkitwidgets \
      network

simulator: warning(This example might not fully work on Simulator platform)

INCLUDEPATH += \
    $(QTDIR)/include \
    /usr/local/include

QMAKE_LIBDIR += \
    /usr/local/lib


QMAKE_INFO_PLIST = ../../deploy/macos/info.plist
