// subversion "$Date: 2019-01-24 15:15:34 -0200 (qui, 24 jan 2019) $"
// subversion "$Author: chico $"
// subversion "$Revision: 109 $"

#ifndef LOGSERVER_H
#define LOGSERVER_H

#include <QWidget>
#include <QString>
#include <QFile>
#include <QTimer>
#include <QPointer>
#include <QSystemTrayIcon>
#include "LogNetSender.h"
#include "ui_LogServer.h"

class QDesktopWidget;

class LogServer : public QWidget , public Ui::LogServerClass    {

	Q_OBJECT

public:
	LogServer(QWidget *parent = 0);
	~LogServer();
	QSize sizeHint() const;

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

private:
	QPoint			dragPosition;
	int				mTelaEmUso;
	QString			mFileName;
	QFile			mArqLog;
	qint64			mPosArq;
	QPointer<QSystemTrayIcon> mTrayIcon;
	QString			mToolTip;
	QTimer			*mTimer;
	QString			mEntradaLog;
	QDesktopWidget	*mDesktop;
	LogNetSender	*mLogSender;

private:
signals:
	void	Log(int, QString, QString);

	private
	slots:
	void onTimer(void);
	void home(int screen);
	void reposicionar(void);
	void trocaVisibilidade(void);
	void trayAtivado(QSystemTrayIcon::ActivationReason reason);
	void receberEstadoInatividade(void);
	void receberEstadoAtividade(void);

	friend
	class LogNetSender;
};

#endif // LOGSERVER_H
