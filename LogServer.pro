# "$Date: 2019-01-11 12:41:52 -0200 (sex, 11 jan 2019) $"
# "$Author: chico $"
# "$Revision: 21 $"

TEMPLATE = app

TARGET = LogServer

QT += core \
	widgets \
	gui \
	network

#Exigido pelo QtBaseGeral/base_geral.pri
VER_MAJ = 0
VER_MIN = 9
VER_PAT = 9
PROJECT_DIR=$$PWD
include (../QtBaseGeral/base_geral.pri)

#INCLUDEPATH += .
#INCLUDEPATH += GeneratedFiles/
#MOC_DIR += GeneratedFiles/
#UI_DIR += GeneratedFiles/
#RCC_DIR += GeneratedFiles/

#debug {
#	INCLUDEPATH += ./GeneratedFiles
#	MOC_DIR += ./GeneratedFiles
#	OBJECTS_DIR += ./GeneratedFiles/debug
#	UI_DIR += ./GeneratedFiles
#	RCC_DIR += ./GeneratedFiles
#	DESTDIR = ./debug
#}

#release {
#	INCLUDEPATH += ./GeneratedFiles
#	MOC_DIR += ./GeneratedFiles
#	OBJECTS_DIR += ./GeneratedFiles/release
#	UI_DIR += ./GeneratedFiles
#	RCC_DIR += ./GeneratedFiles
#	DESTDIR = ./
#}

HEADERS += LogServer.h \
	LogNetSender.h \
	Erros.h

SOURCES += LogServer.cpp \
	LogNetSender.cpp \
	main.cpp

FORMS += LogServer.ui

RESOURCES += LogServer.qrc

win32:RC_FILE = icon.rc

