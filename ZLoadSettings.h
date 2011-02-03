//
// Project: zTaskMan
//
// C++ Implementation: ZSettings
//
// Description: load configuration and information about application
//
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2010-2011
//

#ifndef ZLOADSETTINGS_H
#define ZLOADSETTINGS_H

#include <qcopchannel_qws.h>
#include <qstring.h>
#include <qobject.h>
#include <qmap.h>
#include <qcolor.h>

class BDRegistry
{
public:
    BDRegistry(){}
    ~BDRegistry(){}

	QString icon() const { return icon_; }
	QString name() const { return name_; }
	bool t() const { return t_; }
	void setIcon( QString salary ) { icon_ = salary; }
	void setName( QString salary ) { name_ = salary; }
	void setT( bool salary ) { t_ = salary; } //Mark item from SysRegistry

	#ifdef SWITCH_BY_UID
	QString uid() const { return uid_; }
	void setUid( QString salary ) { uid_ = salary; }
	#endif

private:
	QString icon_;
	QString name_;
	bool t_;
	#ifdef ES_EVENT
	QString uid_;
	#endif
};

typedef QMap<QString, BDRegistry> mapIconNativ;
typedef QMap<QString, QString> mapJavaAppName;

class ZSettings: public QObject
{
Q_OBJECT
public:
	ZSettings();
	~ZSettings();
	
	void reloadSettings();
	void save();
	void loadAppDB();
	
	mapIconNativ mapIcon;
	mapJavaAppName mapJava;
	QString getJavaFolderInSD();
	QString JavaFolderInSD;
		
	#ifdef RAISE_PHONE
	bool cfg_SendReaisePhone;
	bool cfg_SendGoToIDLE;
	#endif
	bool cfg_FiltrProc;
    bool cfg_ShowAppNoWindow;
    bool cfg_GreenShowInfo;
    int cfg_CAction;    
    int cfg_PanelFontSize;	
	int cfg_ListFontSize;
	int cfg_DaemonKey;
	bool cfg_TimeInCaption;
	QString cfg_HideProcList;
	
	QColor cfg_FontColor;
	bool cfg_UserFont;
		
	QString cfg_Language;
	
	//	
	QString getProgramDir();
	void processReg(QString registry);
	#ifndef NEW_JAVA_LIST
	void processIndex(QString index);
	#endif
	
	void startDaemonChenel();
	void showGUI();
	
public slots:
  	void siganalReceived(const QCString&, const QByteArray&);
	
protected:
	#ifdef HARD_KEY_DAEMON
	void timerEvent ( QTimerEvent * );
    bool bIgnoreSignal;
    #define SHOW_EXT_CHECK !bIgnoreSignal
    #else
    #define SHOW_EXT_CHECK true
    #endif
    
    QCopChannel* channelStart;
    QString progDir;
};

#endif
