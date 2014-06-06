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

QT += widgets \
      webkitwidgets \
      network

# install
target.path = ./
INSTALLS += target

simulator: warning(This example might not fully work on Simulator platform)

INCLUDEPATH += \
    $(QTDIR)/include \
    /usr/local/include

QMAKE_LIBDIR += \
    /usr/local/lib
