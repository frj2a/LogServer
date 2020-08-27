// "$Date: 2019-01-24 15:15:34 -0200 (qui, 24 jan 2019) $"
// "$Author: chico $"
// "$Revision: 109 $"

#ifndef _QTLOGNETSENDER_H
#define _QTLOGNETSENDER_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <QDateTime>
#include <QTcpServer>
#include <QStringList>
#include <QMutex>

#define DEFAULT_PORT		2010
#define MAX_ENTRADAS		1000

class LogNetSender : public QObject
{
	Q_OBJECT

public:
	LogNetSender(QWidget *parent = 0, int Filtro = 5, int MaxEntradas = MAX_ENTRADAS, int Porta = DEFAULT_PORT, QStringList *TiposErro = nullptr);
	~LogNetSender();

	//! \brief
	quint16		tcpPort();

	//! \brief
	QString 	tcpAddress();

	//! \brief
	bool		isEnabled();

	//! \brief
	int 		levelOf(QString);

	//! \brief
	bool		isBusy();

	//! \brief
	void		finish();

public
Q_SLOTS:

	//! \brief
	void		Log(int Type, QString Class, QString Message);

private
Q_SLOTS:

	//! \brief
	void		onTimer(void);

	//! \brief
	void		enviaLog(void);

private:
Q_SIGNALS:

	//! \brief
	void		signalNetworkLogActive();

	//! \brief
	void		signalNetworkLogInactive();

private:

	//! \brief
	int			mFiltro;

	//! \brief
	QTimer		*mTimer;

	//! \brief
	int			mNumBuscas;

	//! \brief
	int			mMaxEntradas;

	//! \brief
	QStringList	mEntradasLog;

	//! \brief
	QTcpServer	*tcpServer;

	//! \brief
	QString 	mIpAddress;

	//! \brief
	qint16		mPorta;

	//! \brief
	bool		mHabilitado;

	//! \brief
	QStringList	mNomesTipos;

	//! \brief
	QMutex		* mutex;

	//! \brief
	int			mQuant;

	//! \brief
	int			mCount;

	//! \brief
	QString		mEntrada;

};

#endif // _QTLOGNETSENDER_H
