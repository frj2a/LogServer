@echo off
set PATH=%QT_PATH%;%QT_CREATOR_PATH%;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\Program Files\CollabNet\Subversion Client;C:\MinGW\bin;C:\MinGW\MSYS\1.0\bin;
call "C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"
qmake && jom -j2 clean