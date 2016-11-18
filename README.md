The pydio-sync repository is providing a headless python script for running the synchro.
It's based on the Qt (see http://qt-project.org) library.

## Build instructions

To run it, you must:
 * Install Qt
 * Clone the project
```
git clone git://github.com/pydio/pydio-sync-ui
```
 * Open the .pro file in Qt Creator and build the project

## Manual build instructions
### On Mac OS
 * Generate a makefile from your project file
```
cd pydio-sync-ui/src/systray/
/path/to/qmake systray.pro
```
 * Compile
```
make
```

#### Deployment
```
cd pydio-sync-ui/build
macdeployqt pydio-ui.app -executable=pydio-ui.app/Contents/MacOS/pydio-ui
```

### On Windows
 * If you choose to use Visual Studio tools:
```
REM Create makefile
"C:\Qt\Qt5.7.0-2\5.7\msvc2015_64\bin\qmake.exe" systray.pro -r -spec win32-msvc2015
REM Load required env variables
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
REM Compile
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\nmake.exe" -f Makefile.Release
```

Currently tested on Mac and Windows 10.

## Contributing

Please <a href="http://pyd.io/contribute/cla">sign the Contributor License Agreement</a> before contributing.
