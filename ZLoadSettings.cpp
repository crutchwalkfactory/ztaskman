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

#include "ZLoadSettings.h"

#include "lng.h"
#include "ZDefs.h"
#include "zTaskMan.h"
#include "ZUtils.h"

#include <ZConfig.h>

#include <dirent.h>
#include <iostream>

#ifdef NEW_PLATFORM
#define ICON_NAME "SimpleIcon"
#define EXEC_NAME "ExecId"
#else
#define ICON_NAME "Icon"
#define EXEC_NAME "Exec"
#endif

extern ZLng* lng;
extern ZTaskMan *zTaskMan;

ZSettings::ZSettings()
	:QObject()
{
	//For don't action on Red Button
	disconnect( qApp, SIGNAL(askReturnToIdle(int)), qApp, SLOT(slotReturnToIdle(int)) );
	
	//Get programm dir
	progDir = QString ( qApp->argv()[0] ) ;
	int i = progDir.findRev ( "/" );
	progDir.remove ( i+1, progDir.length() - i );
	lng->ProgDir = progDir;	
	qDebug("cur path: " + progDir);

	reloadSettings();
	JavaFolderInSD = getJavaFolderInSD();
	
	#ifdef HARD_KEY_DAEMON
		#ifdef EZX_Z6
		cfg_DaemonKey = KEY_VR;
		#elif EZX_E8
		cfg_DaemonKey = KEY_POWER;
		#elif EZX_ZN5
		cfg_DaemonKey = KEY_POWER;
		#elif EZX_VE66
		cfg_DaemonKey = 4147;
		#else
		cfg_DaemonKey = 0;
		#endif
		bIgnoreSignal = false;
	#endif
}

ZSettings::~ZSettings()
{
	if (zTaskMan != NULL)
	{
		delete zTaskMan;
		zTaskMan = NULL;
	}
}

QString ZSettings::getJavaFolderInSD()
{
	toLog("getJavaFolderInSD()");
	static DIR *dir;
	struct dirent *entry;
	QString qname;		

	dir = opendir("/mmc/mmca1/.system");
	if(!dir)
	{
		toLog("Can't open SD Card");
	} else	
	{
		for(;;) 
		{	
			if((entry = readdir(dir)) == NULL) 
			{
				closedir(dir);
				dir = 0;
				break;
			}	
			
			qname = QString(entry->d_name);
			
			if (qname.find("java_")>-1)
			{
				toLog("JavaFolderInSD: "+ QString(qname));
				return qname;
			}
		}
	}
	toLog("JavaFolderInSD: not found");
	return NULL;	
}

void ZSettings::processReg(QString registry)
{
	QString ico;
	QString nname;
	
	ZConfig confREG ( registry, false );
	QStringList grouplist;
	confREG.getGroupsKeyList ( grouplist );
	for ( QStringList::Iterator it = grouplist.begin(); it != grouplist.end(); ++it ) 
	{	 
		#ifndef NEW_PLATFORM
		if ( confREG.readEntry ( *it, "JavaId", "" ) != ""  )
		#else
		if ( confREG.readNumEntry( *it, "AppType", 0 ) != 0 )
		#endif
			continue;
		
		nname = confREG.readEntry ( *it, EXEC_NAME, "" );
		ico = confREG.readEntry ( *it, ICON_NAME, "" );
		if ( ico.isEmpty() )
			ico = confREG.readEntry ( *it, "BigIcon", "" );

		if ( nname!="" && ico!="" )
		{
			BDRegistry app;
			app.setIcon(ico);
			app.setName(confREG.readEntry ( *it, "Name", "" ));
			app.setT(false);
			#ifdef SWITCH_BY_UID
			app.setUid( *it );
			#endif			
			mapIcon[ nname ] = app;
		}
	}	
}

#ifndef NEW_JAVA_LIST
void ZSettings::processIndex(QString index)
{
	char name[255];
	int id;
	int i;
	
	FILE * f = fopen( QCString( index ), "rt" );
	toLog("processIndex: "+index);
	if ( f )
	{
		fscanf(f, "%d||", &id);
		fgets(name, 255, f);
		while ( !feof(f) )
		{
			for (i=0;i<255;i++)
				if ( name[i] == '|' )
				{
					name[i]=0;
					break;
				}
			mapJava[ QString::number(id) ] = QString(name);
			fscanf(f, "%d||", &id);
			fgets(name, 255, f);
		}
		mapJava[ QString::number(id) ] = QString(name);
		fclose(f);
	} else
	{
		toLog("processIndex: Error! File not found!");
	}	
}
#endif

void ZSettings::loadAppDB()
{
	//*********************** Build list of nativ **********************
	toLog("Build list of icon");
	
	mapIcon.clear();
	
	processReg( CARD_REGISTRY );
	#ifdef OLD_PLATFORM
	processReg( USER_REGISTRY );
	#endif
	processReg( SYS_REGISTRY );

	#ifndef NEW_JAVA_LIST
	//********************* Build list of java name ********************
	toLog("Build list of java name");

	mapJava.clear();
	
	if ( JavaFolderInSD != NULL )
	{
		#ifdef INDEX_CARD1
		processIndex("/mmc/mmca1/.system/"+ JavaFolderInSD +"/JAMInfo/index_card1");
		#else
		processIndex("/mmc/mmca1/.system/"+ JavaFolderInSD +"/JAMInfo/index");
		#endif
	}
	
	processIndex("/ezxlocal/download/mystuff/.system/java/JAMInfo/index");
	#endif
}

void ZSettings::reloadSettings()
{	
	ZConfig cfg(appConf);
	
	#ifdef RAISE_PHONE
	cfg_SendReaisePhone = cfg.readBoolEntry(QString("General"), QString("SendReaisePhone"), true);
	cfg_SendGoToIDLE = cfg.readBoolEntry(QString("General"), QString("SendGoToIDLE"), false);
	#endif
	cfg_Language = cfg.readEntry(QString("General"), QString("Lng"), "");
	if ( !cfg_Language.isEmpty() )
	{
		lng->loadLngFile( progDir+"lng/"+cfg_Language );
		lng->setCurrentLng( cfg_Language );
	} else
		lng->autoLoadLng();		
	cfg_FiltrProc = cfg.readBoolEntry(QString("Task"), QString("FiltrProc"), true);
	cfg_HideProcList = cfg.readEntry(QString("Task"), QString("FiltrProcList"), QString(HIDE_PROCESS));
	cfg_ShowAppNoWindow = cfg.readBoolEntry(QString("Task"), QString("ShowAppNoWindow"), false);
	cfg_PanelFontSize = cfg.readNumEntry(QString("Font"), QString("PanelFontSize"), 14);
	#ifdef OLD_PLATFORM
	cfg_ListFontSize = cfg.readNumEntry(QString("Font"), QString("ListFontSize"), 18);
	#else
	cfg_ListFontSize = cfg.readNumEntry(QString("Font"), QString("ListFontSize"), 16);	
	#endif
	cfg_GreenShowInfo = cfg.readBoolEntry(QString("Key"), QString("ShowInfoByGreen"), true);
    cfg_CAction = cfg.readNumEntry(QString("Key"), QString("ActionByC"), 0); 
    
    cfg_TimeInCaption = cfg.readBoolEntry(QString("General"), QString("TimeInCaption"), false); 

	int r,g,b;
	r = cfg.readNumEntry(QString("Font"), QString("PanelColor_R"), 0);
	g = cfg.readNumEntry(QString("Font"), QString("PanelColor_G"), 0);
	b = cfg.readNumEntry(QString("Font"), QString("PanelColor_B"), 0);
	cfg_FontColor.setRgb(r,g,b);
	
	cfg_UserFont = cfg.readBoolEntry(QString("Font"), QString("UserFont"), false);
}

void ZSettings::save()
{
	qDebug("Save settings");
	ZConfig cfg(appConf);
	
	cfg.writeEntry("Task", "FiltrProc", cfg_FiltrProc );
	cfg.writeEntry("Task", "ShowAppNoWindow", cfg_ShowAppNoWindow );
	cfg.writeEntry("Task", "FiltrProcList", cfg_HideProcList );
	#ifdef RAISE_PHONE
	cfg.writeEntry("General", "SendReaisePhone", cfg_SendReaisePhone );  
	cfg.writeEntry("General", "SendGoToIDLE", cfg_SendGoToIDLE );
	#endif
	cfg.writeEntry("General", "Lng", cfg_Language );
	cfg.writeEntry("Font", "ListFontSize", cfg_ListFontSize );
	cfg.writeEntry("Font", "PanelFontSize", cfg_PanelFontSize );
	cfg.writeEntry("Key", "ShowInfoByGreen", cfg_GreenShowInfo);
    cfg.writeEntry("Key", "ActionByC", cfg_CAction);
    cfg.writeEntry("General", "TimeInCaption", cfg_TimeInCaption);	
    
    cfg.writeEntry("Font", "UserFont", cfg_UserFont );
	cfg.writeEntry("Font", "PanelColor_R", cfg_FontColor.red());
	cfg.writeEntry("Font", "PanelColor_G", cfg_FontColor.green());
	cfg.writeEntry("Font", "PanelColor_B", cfg_FontColor.blue());    
	
	cfg.flush();
}

QString ZSettings::getProgramDir()
{
	return progDir;
}

void ZSettings::startDaemonChenel()
{
	channelStart = new QCopChannel(ZTASKMAN_START,this,"zTaskMan");
    CHECK_PTR(channelStart);
    connect( channelStart, SIGNAL(received(const QCString&, const QByteArray&)), this, SLOT( siganalReceived(const QCString&, const QByteArray&) ) );
    #ifdef HARD_KEY_DAEMON
    QCopChannel * pkeyChannel = new QCopChannel(HARD_KEY_CHENEL, this);
	connect(pkeyChannel, SIGNAL(received(const QCString &,const QByteArray &)), this, SLOT( siganalReceived(const QCString&, const QByteArray&) ) ); 		
	#endif
}

void ZSettings::showGUI()
{
	if (zTaskMan == NULL)
		zTaskMan = new ZTaskMan ( NULL, NULL );
	else
		zTaskMan->Show();
}

#ifdef HARD_KEY_DAEMON
void ZSettings::timerEvent ( QTimerEvent * )
{
	bIgnoreSignal = false;
}
#endif

void ZSettings::siganalReceived( const QCString &msg, const QByteArray &data )
{
	#ifdef HARD_KEY_DAEMON
    if( (msg == "keyMsg(int,int)") )
    {
		int key, type;
		QDataStream stream(data, IO_ReadOnly);
		stream >> key >> type;
		std::cout << "Key "<< key << ", type " << type << std::endl;
		if( key == cfg_DaemonKey && type == KEY_MSG_PRESS )
		{
			killTimers();
			if (zTaskMan == NULL)
				zTaskMan = new ZTaskMan ( NULL, NULL );	
			else
				zTaskMan->Show();
			bIgnoreSignal = true;
			startTimer(2000);
		}
    } else
    #endif
	if ( (msg == "show()" || msg == "newshow()") && SHOW_EXT_CHECK ) 
	{
		if (zTaskMan == NULL)
			zTaskMan = new ZTaskMan ( NULL, NULL );	
		else
			zTaskMan->Show();
	}
}
