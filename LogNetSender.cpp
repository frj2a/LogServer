// subversion "$Date: 2019-02-07 10:48:04 -0200 (qui, 07 fev 2019) $"
// subversion "$Author: chico $"
// subversion "$Revision: 141 $"

#include "LogNetSender.h"

#include <QWidget>
#include <QTimer>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#ifdef _DBG_PV
// #	include <QDebug>
#endif

#define	NUM_MAX_PARA_VAGO		10
#define	INTERVALO_TIMER			50
#define QUANTIDADE_ACUMULADA	10

LogNetSender::LogNetSender(QWidget *parent, int Filtro, int MaxEntradas, int Porta, QStringList *TiposErro ) :	QObject(parent) {
	mHabilitado = true;
	tcpServer = new QTcpServer(this);

	mFiltro = Filtro;
	mMaxEntradas = MaxEntradas;
	mPorta = Porta;

	if (TiposErro == nullptr )	{
		mNomesTipos.clear();
		mNomesTipos
		// nível 1
				<< "Emergencia"		//	Emergency
		// nível 2
				<< "ErroCritico"	//	CriticalError
		// nível 3
				<< "Erro"			//	Error
		// nível 4
				<< "Alerta"			//	Alert
		// nível 5
				<< "Advertencia"	//	Warning
		// nível 6
				<< "Observacao"		//	Note
		// nível 7
				<< "Informacao"		//	Information
		// nível 8
				<< "Depuracao"		//	Debug
		// fim da lista
		;
	} else {
		mNomesTipos = *TiposErro;
	}

	if (!tcpServer->listen(QHostAddress::Any, mPorta)) {
		QMessageBox *msgbox = new QMessageBox(QMessageBox::Critical,
				"LogNetSender",
				tr("Não foi possí­vel iniciar o servidor:\n%1.")
				.arg(tcpServer->errorString()),
				QMessageBox::Ok, parent);
		msgbox->exec();
		mHabilitado = false;
		// return;
	} else {
		mHabilitado = true;
	}

	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

	// use the first non-localhost IPv4 address
	for (int i = ipAddressesList.size() - 1; i >= 0; --i) {
		if ( ( ipAddressesList.at(i) != QHostAddress::LocalHost ) && ( ipAddressesList.at(i).toIPv4Address() ) ) {
			mIpAddress = ipAddressesList.at(i).toString();
		}

	}

	// if we did not find one, use IPv4 localhost
	if (mIpAddress.isEmpty())
		mIpAddress = QHostAddress(QHostAddress::LocalHost).toString();

	connect(tcpServer, &QTcpServer::newConnection, this, &LogNetSender::enviaLog);

	mTimer = new QTimer(this);
	connect(mTimer, &QTimer::timeout, this, &LogNetSender::onTimer);
	mTimer->setSingleShot(false);
	mTimer->setInterval(INTERVALO_TIMER);

	mEntradasLog.clear();

	mNumBuscas = 0;

	mutex = new QMutex(QMutex::Recursive);
	Log(8,"LogNetSender","Iniciado.");
}

LogNetSender::~LogNetSender() {
	tcpServer->close();
	disconnect(tcpServer, &QTcpServer::newConnection, this, &LogNetSender::enviaLog);

	delete mutex;
	delete tcpServer;

	if (mTimer != nullptr) {
		delete mTimer;
	}
}

quint16	LogNetSender::tcpPort()	{
	return tcpServer->serverPort()	;
}

QString LogNetSender::tcpAddress()	{
	return mIpAddress;
}

int LogNetSender::levelOf(QString errorName){
	return (mNomesTipos.indexOf(errorName) + 1);
}

bool LogNetSender::isEnabled()	{
	return mHabilitado;
}

bool LogNetSender::isBusy()	{
	return (mEntradasLog.size() > 0);
}

void LogNetSender::finish() {
	mTimer->stop();
	disconnect(mTimer, &QTimer::timeout, this, &LogNetSender::onTimer);
	mHabilitado = false;
}

void LogNetSender::Log(int Type, QString Class, QString Message) {
	mutex->lock();
	{	// indentação para destacar a abrangência do "mutex" acima;
		if (mFiltro >= Type) {
			mEntrada.clear();
			mEntrada.append(QDateTime::currentDateTime().toString("yyyy-MMM-dd hh:mm:ss.zzz"));
			mEntrada.append(" TP:");
			mEntrada.append(mNomesTipos.at(Type - 1));	// Tip começa em 1
			mEntrada.append(" OR:");
			mEntrada.append(Class);
			mEntrada.append(" RG:");
			mEntrada.append(Message);
			mEntrada.append("\n");
			mEntradasLog.append(mEntrada);
			if (mEntradasLog.count() > mMaxEntradas)	{
				mEntradasLog.pop_back();
			}
		}
	}
	// qDebug() << mEntradasLog.size() << "acumulando";
	mutex->unlock();
	qApp->processEvents();
}

void LogNetSender::onTimer(void) {
	if (mNumBuscas >= NUM_MAX_PARA_VAGO) {
		emit signalNetworkLogInactive();
		mHabilitado = false;
		mTimer->stop();
	} else {
		mNumBuscas++;
	}
}

void LogNetSender::enviaLog() {
	mNumBuscas = 0;
	if ( (mTimer != nullptr) && (!mTimer->isActive()) ) {
		emit signalNetworkLogActive();
		mHabilitado = true;
		mTimer->start();
	}
	mutex->lock();
	{
		QByteArray block;
		QDataStream saida(&block, QIODevice::WriteOnly);

		saida.setVersion(QDataStream::Qt_5_11);

		if (mEntradasLog.count() > 0 ) {
			mQuant = (mEntradasLog.count() > QUANTIDADE_ACUMULADA ? QUANTIDADE_ACUMULADA : mEntradasLog.count());

			for (mCount = 0; mCount < mQuant; mCount++) {
				saida << mEntradasLog.takeFirst();
			}
		}

		QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
		connect(clientConnection, &QTcpSocket::disconnected, clientConnection, &QTcpSocket::deleteLater);

		clientConnection->write(block);
		clientConnection->disconnectFromHost();

	}
	// qDebug() << mEntradasLog.size() << "acumulados.";
	mutex->unlock();
}
