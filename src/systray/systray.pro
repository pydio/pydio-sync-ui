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
                httppoller.h \
                job.h
SOURCES       = main.cpp \
                window.cpp \
                queuemenu.cpp \
                portconfigurer.cpp \
                JSEventHandler.cpp \
                httppoller.cpp \
                job.cpp

RESOURCES     = systray.qrc

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
