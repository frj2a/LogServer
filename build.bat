@echo off
REM "$Date: 2019-01-24 16:35:14 -0200 (qui, 24 jan 2019) $"
REM "$Author: chico $"
REM "$Revision: 124 $"
copy /y Erros.h ..\LogNetSender\
copy /y LogNetSender.h ..\LogNetSender\
copy /y LogNetSender.cpp ..\LogNetSender\
set OBJECT=LogServer
del /q %OBJECT%.exe
del /q %OBJECT%.upx
rem set PATH=C:\Qt\4.8.3.static\bin;%QT_CREATOR_PATH%;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;"C:\Program Files\Subversion\bin";C:\MinGW\bin;C:\MinGW\MSYS\1.0\bin;
rem set PATH=%QT_PATH%;%QT_CREATOR_PATH%;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;"C:\Program Files\Subversion\bin";C:\MinGW\bin;C:\MinGW\MSYS\1.0\bin;
rem call "C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"
rem qmake -r -spec win32-msvc2005 "CONFIG-=release debug" "CONFIG+=release static staticlibs" && jom -j2 release && call \UPX.bat release\LogServer.exe && move /Y release\LogServer.exe .\ && call distclean
svn up
\Qt\Qt-5.12.0\bin\lrelease *.pro
qmake -r -spec win32-g++ "CONFIG-=debug_and_release release debug" "CONFIG+=release"  && mingw32-make qmake_all && mingw32-make -s -j4 && strip -psx %OBJECT%.exe && "C:\Program Files\upx-3.95-win64\upx.exe" --ultra-brute --best %OBJECT%.exe && move %OBJECT%.exe ..\ && call distclean.bat && move ..\%OBJECT%.exe .\