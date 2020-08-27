# "$Date: 2019-01-24 09:36:25 -0200 (qui, 24 jan 2019) $"
# "$Author: chico $"
# "$Revision: 85 $"

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

op=$$find(QT, "network")
count(op,0) {
	QT += network
}

HEADERS	+= \
		$$PWD/LogNetSender.h \
		$$PWD/Erros.h

SOURCES	+= \
		$$PWD/LogNetSender.cpp

DEFINES += _LogSender
