HEADERS      += window.h \
                Subscriber.hpp \
                SampleBase.hpp \
                ../../nzmqt/include/nzmqt/nzmqt.hpp \
                pydiogui.h \
                queuemenu.h \
                portconfigurer.h \
                togglestatusrequester.h \
                smoketest.h
SOURCES       = main.cpp \
                window.cpp \
                pydiogui.cpp \
                queuemenu.cpp \
                portconfigurer.cpp \
                togglestatusrequester.cpp \
                smoketest.cpp
RESOURCES     = systray.qrc

QT += widgets \
      webkitwidgets

LIBS += -lzmq

# install
target.path = ./
INSTALLS += target

simulator: warning(This example might not fully work on Simulator platform)

INCLUDEPATH += \
    ../../nzmqt/include \
    ../../nzmqt/externals/include \
    $(QTDIR)/include \
    /usr/local/include

QMAKE_LIBDIR += \
    /usr/local/lib

FORMS += \
    pydiogui.ui
