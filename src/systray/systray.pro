win32 {
QT_DIR = C:\Qt\5.2.1
ZMQ_DIR = C:\Program Files\ZeroMQ\ 4.0.4
}

HEADERS       = window.h \
                Subscriber.hpp \
                SampleBase.hpp \
                ../../nzmqt/include/nzmqt/nzmqt.hpp \
                Requester.hpp \
                pydiogui.h
SOURCES       = main.cpp \
                window.cpp \
                pydiogui.cpp
RESOURCES     = systray.qrc

QT += widgets \
      webkitwidgets

# install
target.path = ./
INSTALLS += target

simulator: warning(This example might not fully work on Simulator platform)

win32:CONFIG(release, debug|release) {
LIBS += "$$ZMQ_DIR\bin\libzmq-v120-mt-4_0_4.dll"
INCLUDEPATH += \
    ../../nzmqt/include \
    ../../nzmqt/externals/include \
    $$QT_DIR\mingw48_32\include \
    $$ZMQ_DIR\include
QMAKE_LIBDIR += \
    $$ZMQ_DIR\bin \
    $$QT_DIR\mingw48_32\bin \
    $$QT_DIR\mingw48_32\lib
} else:win32:CONFIG(debug, debug|release) {
LIBS += "$$ZMQ_DIR\bin\libzmq-v120-mt-gd-4_0_4.dll"
INCLUDEPATH += \
    ../../nzmqt/include \
    ../../nzmqt/externals/include \
    $$QT_DIR\mingw48_32\include \
    $$ZMQ_DIR\include
QMAKE_LIBDIR += \
    $$ZMQ_DIR\bin \
    $$QT_DIR\mingw48_32\bin \
    $$QT_DIR\mingw48_32\lib
} else:unix {
INCLUDEPATH += \
    ../../nzmqt/include \
    ../../nzmqt/externals/include \
    /usr/local/include \
QMAKE_LIBDIR += \
    /usr/local/lib \
}


FORMS += \
    pydiogui.ui


