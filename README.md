The pydio-sync repository is providing a headless python script for running the synchro.
It's based on the Qt (see http://qt-project.org) library.

Since some changes have occurred with Qt 5, we recommend to install the latest version  !

## Build instructions

To run it, you must 
 * Install Qt 5.2
 * Clone the project
```
git clone git://github.com/pydio/pydio-sync-ui
```
 * Open the .pro file in Qt Creator and build the project or :

### On Mac OS
```
export PATH=$PATH:/your/path/to/qmake
```
 * Generate a makefile from your project file
```
cd /your/path/to/pydio-sync-ui/src/systray/
qmake -o path/you/want/makefile/to/go systray.pro
```
 * Compile
```
make
```

#### Deployment
```
cd ../../build
macdeployqt pydio-ui.app -no-plugins
open pydio-ui.app
```

### On Windows
 * Generate a makefile from your project file
```
c:/qt/5.2.1/mingw48_32/bin/./qmake your/path/to/pydio-sync-ui/src/systray/systray.pro
```
 * Compile
```
C:/Qt/Tools/mingw48_32/bin/./mingw32-make -f makefile
```
* Add the following .dll files to the systray.exe folder :
```
Qt5Core.dll
Qt5Gui.dll
Qt5Multimedia.dll
Qt5MultimediaWidgets.dll
Qt5Network.dll
Qt5OpenGL.dll
Qt5Positioning.dll
Qt5PrintSupport.dll
Qt5Qml.dll
Qt5Quick.dll
Qt5Sensors.dll
Qt5Sql.dll
Qt5WebKit.dll
Qt5WebKitWidgets.dll
Qt5Widgets.dll
icudt51.dll
icuin51.dll
icuuc51.dll
```
* Launch the UI

Currently tested on Mac and Windows 7.

## Contributing

Please <a href="http://pyd.io/contribute/cla">sign the Contributor License Agreement</a> before contributing.
