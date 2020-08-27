// subversion "$Date: 2019-01-24 15:15:34 -0200 (qui, 24 jan 2019) $"
// subversion "$Author: chico $"
// subversion "$Revision: 109 $"

#define REV_MAJOR_CODE "0"
#define REV_MINOR_CODE "0"
#define REV_PATCH_CODE "1"

#include "LogServer.h"
#include <QtGui>
#include <QRect>
#include <QTextStream>
#include <QDesktopWidget>
#include <QScreen>
#include <QMenu>

#define	INTERVALO_TIMER		500

LogServer::LogServer(QWidget *parent) :
QWidget(parent, Qt::FramelessWindowHint | Qt::SplashScreen
| Qt::WindowStaysOnTopHint), Ui::LogServerClass() {

	setupUi(this);
	setAttribute(Qt::WA_TranslucentBackground, true);

	mToolTip = tr("Drag the icon with the mouse left button.\n\n"
	"Use right button to open a context menu.\n\n"
	"The conection is: ");

	qApp->setWindowIcon(QIcon(":/images/icon"));

	mTrayIcon = new QSystemTrayIcon(QIcon(":/images/SysTray"), this);
	mTrayIcon->setVisible(true);
	mTrayIcon->show();
	connect(mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayAtivado(QSystemTrayIcon::ActivationReason)));

	mTelaEmUso = qApp->desktop()->screenNumber(this);
	reposicionar();

	mFileName = qApp->arguments().at(1);
	mArqLog.setFileName(mFileName);
	mPosArq = 0;

	QAction *hideAction = new QAction(tr("&Show/Hide"), mTrayIcon);
	hideAction->setShortcut(tr("Ctrl+S"));
	connect(hideAction, SIGNAL(triggered()), this, SLOT(trocaVisibilidade()));
	addAction(hideAction);

	QAction *homeAction = new QAction(tr("&Reposition"), this);
	homeAction->setShortcut(tr("Ctrl+R"));
	connect(homeAction, SIGNAL(triggered()), this, SLOT(reposicionar()));
	addAction(homeAction);

	QAction *quitAction = new QAction(tr("E&xit"), this);
	quitAction->setShortcut(tr("Ctrl+X"));
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	addAction(quitAction);

	mDesktop = qApp->desktop();
	connect (mDesktop, SIGNAL(resized(int)), this, SLOT(home(int)) );

	mLogSender = new LogNetSender(this, 10);
	connect(this, SIGNAL(Log(int, QString, QString)), mLogSender, SLOT(Log(int, QString, QString)));
	connect(mLogSender, SIGNAL(signalNetworkLogActive()), this, SLOT(receberEstadoAtividade()));
	connect(mLogSender, SIGNAL(signalNetworkLogInactive()), this, SLOT(receberEstadoInatividade()));

	QMenu trayIconMenu;
	trayIconMenu.addAction(hideAction);
	//	trayIconMenu->addAction(homeAction);
	trayIconMenu.addSeparator();
	trayIconMenu.addAction(quitAction);

	mTrayIcon->setContextMenu(&trayIconMenu);

	mTimer = new QTimer(this);
	connect (mTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	mTimer->setSingleShot(false);
	mTimer->setInterval(INTERVALO_TIMER);
	mTimer->start();

	receberEstadoInatividade();
}

LogServer::~LogServer() {
	mTimer->stop();
	disconnect (mTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	delete mTimer;

	disconnect(mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayAtivado(QSystemTrayIcon::ActivationReason)));
	delete mTrayIcon;

	disconnect(this, SIGNAL(Log(int, QString, QString, QString)), mLogSender, SLOT(Log(int, QString, QString, QString)));
	disconnect(mLogSender, SIGNAL(signalNetworkLogActive()), this, SLOT(receberEstadoAtividade()));
	disconnect(mLogSender, SIGNAL(signalNetworkLogInactive()), this, SLOT(receberEstadoInatividade()));
	delete mLogSender;
}

void LogServer::trocaVisibilidade() {
	if (isVisible()) {
		hide();
	} else {
		show();
		reposicionar();
	}
}

void LogServer::trayAtivado(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger) {
		trocaVisibilidade();
	}
}

void LogServer::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		dragPosition = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	}
}

void LogServer::mouseMoveEvent(QMouseEvent *event) {
	if (event->buttons() & Qt::LeftButton) {
		move(event->globalPos() - dragPosition);
		event->accept();
	}
}

QSize LogServer::sizeHint(void) const {
	return QSize(labelLogo->size());
}

void LogServer::reposicionar() {
	home(mTelaEmUso);
}

void LogServer::home(int tela) {
	if (tela == mTelaEmUso) {
		QRect deskArea = qApp->desktop()->screen(mTelaEmUso)->geometry();
		int x, y, w, h;
		w = geometry().width();
		h = geometry().height();
		x = deskArea.width() * 125 / 128 - w;
		y = deskArea.height() * 122 / 128 - h;
		setGeometry(x, y, w, h);
	}
}

void LogServer::receberEstadoAtividade() { // slot
	setToolTip(QString(mToolTip).append(tr("ACTIVE in %1@%2")).arg(mLogSender->tcpAddress()).arg(mLogSender->tcpPort()));
	labelLogo->setPixmap(QPixmap(":images/ativo"));
	mTrayIcon->showMessage(tr("Aviso"),tr("Online.\n%1@%2").arg(mLogSender->tcpAddress()).arg(mLogSender->tcpPort()), QSystemTrayIcon::Information,400);
}

void LogServer::receberEstadoInatividade() { // slot
	setToolTip(QString(mToolTip).append(tr("INACTIVE in %1@%2")).arg(mLogSender->tcpAddress()).arg(mLogSender->tcpPort()));
	labelLogo->setPixmap(QPixmap(":images/inativo"));
	mTrayIcon->showMessage(tr("Aviso"),tr("Offline.\n%1@%2").arg(mLogSender->tcpAddress()).arg(mLogSender->tcpPort()), QSystemTrayIcon::Information,400);
}

void LogServer::onTimer(void)	{
	if (mArqLog.open(QIODevice::ReadOnly | QIODevice::Text))	{
		QTextStream tmpTextStream(&mArqLog);
		tmpTextStream.seek(mPosArq);

		if (tmpTextStream.atEnd()) {
			tmpTextStream.seek(0);
			mPosArq=0;
		}

		mEntradaLog = tmpTextStream.readLine();
		mPosArq = tmpTextStream.pos();

		mArqLog.close();
	}
	QStringList strings = mEntradaLog.split(";",QString::KeepEmptyParts);

	// emit Log(mLogSender->levelOf(strings.at(1)), strings.at(2));
	emit Log(strings.at(0).toInt(), QString(strings.at(1)), QString(strings.at(2)));
}
