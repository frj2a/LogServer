@echo off
REM "$Date: 2019-01-24 16:35:14 -0200 (qui, 24 jan 2019) $"
REM "$Author: chico $"
REM "$Revision: 124 $"
set OBJECT_=LogServer
mingw32-make -j4 -s distclean
del /s /q release
rmdir /s /q release
del /s /q debug
rmdir /s /q debug
del /s /q GeneratedFiles
rmdir /s /q GeneratedFiles
Z: && cd Z:\ && rmdir /s /q %OBJECT_%
C: