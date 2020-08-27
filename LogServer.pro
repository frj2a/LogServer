# "$Date: 2019-01-11 12:41:52 -0200 (sex, 11 jan 2019) $"
# "$Author: chico $"
# "$Revision: 21 $"

TEMPLATE = app

TARGET = LogServer

QT += core \
	widgets \
	gui \
	network

INCLUDEPATH += .
INCLUDEPATH += GeneratedFiles/
MOC_DIR += GeneratedFiles/
UI_DIR += GeneratedFiles/
RCC_DIR += GeneratedFiles/

debug {
	INCLUDEPATH += ./GeneratedFiles
	MOC_DIR += ./GeneratedFiles
	OBJECTS_DIR += ./GeneratedFiles/debug
	UI_DIR += ./GeneratedFiles
	RCC_DIR += ./GeneratedFiles
	DESTDIR = ./debug
}

release {
	INCLUDEPATH += ./GeneratedFiles
	MOC_DIR += ./GeneratedFiles
	OBJECTS_DIR += ./GeneratedFiles/release
	UI_DIR += ./GeneratedFiles
	RCC_DIR += ./GeneratedFiles
	DESTDIR = ./
}

HEADERS += LogServer.h \
	LogNetSender.h \
	Erros.h

SOURCES += LogServer.cpp \
	LogNetSender.cpp \
	main.cpp

FORMS += LogServer.ui

RESOURCES += LogServer.qrc

win32:RC_FILE = icon.rc

win32 {
	defineReplace(Revisions)	{
		variable = $$1
		ALLFILES = $$eval($$variable)
		NUMBERS = $$system(set LC_ALL="C" && svn info | sed --quiet /Rev:/p | cut -d: -f2 )
		# message($$NUMBERS)
		return ($$NUMBERS)
	}
}

unix {
	defineReplace(Revisions)	{
		NUMBERS = $$system(B=`LC_ALL="C" svn info | grep "Rev:" | cut -d\" \"   -f4` && echo $B)
		# message($$NUMBERS)
		return ($$NUMBERS)
	}
}

DEFINES += REV_SUBVN_CODE=$$Revisions()
