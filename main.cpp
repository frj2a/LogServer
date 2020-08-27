// subversion "$Date: 2019-01-11 12:41:52 -0200 (sex, 11 jan 2019) $"
// subversion "$Author: chico $"
// subversion "$Revision: 21 $"

#include "LogServer.h"

#include <QtGui>
#include <QApplication>
#include <QString>
#include <QFile>
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if (a.arguments().count() > 1)	{
		QString mFileName = qApp->arguments().at(1);
		QFile mArqLog;
		mArqLog.setFileName(mFileName);
		if (!mArqLog.exists() || mFileName.isEmpty())	{
			QMessageBox msgbox(QMessageBox::Critical,
							   QObject::tr("Arquivo não encontrado"),
							   mFileName,QMessageBox::Ok,0);
							   msgbox.setDetailedText(QObject::tr("Verifique a sintaxe."));
							   msgbox.setModal(true);
							   msgbox.exec();
							   return 1;
		} else {
			LogServer w(0);
			w.show();
			return a.exec();
		}
	} else {
		return 1;
	}
}
