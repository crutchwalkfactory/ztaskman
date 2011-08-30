//
// Project: zTaskMan
//
// C++ Implementation: ZAppSetting
//
// Description: setting dlg
//
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2008-2011
//

#include "ZSettingDlg.h"

#include "zTaskMan.h"
#include "lng.h"
#include "ZLoadSettings.h"
#include "ZDefs.h"
#include "ZUtils.h"
#include "ZOptionItem.h"

#include <ZSoftKey.h>
#include <ZConfig.h>

#include <qdir.h>

extern ZTaskMan *zTaskMan;
extern ZLng* lng;
extern ZSettings* settings;

ZAppSetting::ZAppSetting()
{
	toLog("ZSettingDlg: Start");
	
	setMainWidgetTitle(lng->getString("DLG_SETTING"));
	
	zlbSettings = new ZListBox(this);
	this->setContentWidget(zlbSettings);

	toLog("ZSettingDlg: Create CheckBox and Load settings");
	
	ZOptionItem * item;
	
	//++++++++++++++++++++
	#ifdef AUTORUN
	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_BOOL_ONOFF, 0);
	item->setTitle(lng->getString("DAEMON"));
	item->setNum(false);
	zlbSettings->insertItem ( item,-1 );

	ZConfig config("/ezxlocal/download/appwrite/am/UserConfig");
	QString sAutoRunApps = config.readEntry("AutoRun", "Apps");
	QStringList list = QStringList::split( ';', sAutoRunApps);
	QStringList::Iterator it = list.find(AUTORUN_APPID);
	if ( list.end() != it )
		item->setNum(true);
	#endif
	//++++++++++++++++++++
	
	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_BOOL_ONOFF, 1);
	item->setTitle(lng->getString("PROCFILTER"));
	item->read(APP_CONF_NAME, "Task", "ShowAllTask", false);
	zlbSettings->insertItem ( item,-1 );	

	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_BOOL_ONOFF, 2);
	item->setTitle(lng->getString("TASKNOWIN"));
	item->read(APP_CONF_NAME, "Task", "ShowAppNoWindow", false);
	zlbSettings->insertItem ( item,-1 );	

	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_BOOL_ONOFF, 3);
	item->setTitle(lng->getString("TIMEINCAPTION"));
	item->read(APP_CONF_NAME, "General", "TimeInCaption", false);
	zlbSettings->insertItem ( item,-1 );		
	
	//++++++++++++++++++++
	#ifdef RAISE_PHONE
	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_BOOL_ONOFF, 4);
	item->setTitle(lng->getString("SENDREASEPHONE"));
	item->read(APP_CONF_NAME, "Task", "SendReaisePhone", true);
	zlbSettings->insertItem ( item,-1 );		
	
	
	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_BOOL_ONOFF, 5);
	item->setTitle(lng->getString("SENDTOIDLE"));
	item->read(APP_CONF_NAME, "Task", "SendGoToIDLE", false);
	zlbSettings->insertItem ( item,-1 );
	#endif
	
	//++++++++++++++++++++
	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_BOOL_ONOFF, 6);
	item->setTitle(lng->getString("SHOWINFOGREEN"));
	item->read(APP_CONF_NAME, "Key", "ShowInfoByGreen", true);
	zlbSettings->insertItem ( item,-1 );

	QStringList * qslActionC = new QStringList();
    qslActionC->append(lng->getString("NO"));
	qslActionC->append(lng->getString("KILL"));
	qslActionC->append(lng->getString("QUIT"));
	
	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_ONE_OF_LIST, 7);
	item->setTitle(lng->getString("ACTIONC"));
	item->setList( qslActionC );
	item->read(APP_CONF_NAME, "Key", "ActionOnC", 0);
	zlbSettings->insertItem ( item,-1 );	
	
	//++++++++++++++++++++
	QStringList * qslLangList = new QStringList();
	int iCur = lng->getLngList( qslLangList );
	
	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_ONE_OF_LIST, 8);
	item->setTitle(lng->getString("LANG"));
	item->setList( qslLangList );
	item->setNum( iCur );
	zlbSettings->insertItem ( item,-1 );
	
	//++++++++++++++++++++
	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_NUM, 9);
	item->setTitle(lng->getString("LISTFONTSIZE"));
	item->setMaxMin(30, 6);	
	item->read(APP_CONF_NAME, "Font", "ListFontSize", 16);
	zlbSettings->insertItem ( item,-1 );	
	
	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_NUM, 10);
	item->setTitle(lng->getString("PANELFONTSIZE"));
	item->setMaxMin(30, 6);	
	item->read(APP_CONF_NAME, "Font", "PanelFontSize", 14);
	zlbSettings->insertItem ( item,-1 );
	
	#ifndef NO_CHANGE_COLOR	
	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_BOOL_ONOFF, 11);
	item->setTitle(lng->getString("USEUSERCOLOR"));
	item->read(APP_CONF_NAME, "Key", "UserFont", false);
	zlbSettings->insertItem ( item,-1 );

	item = new ZOptionItem(zlbSettings, ZOptionItem::EDIT_COLOR, 12);
	item->setTitle(lng->getString("COLOR"));
	item->read(APP_CONF_NAME, "Font", "PanelColor", "#000000");
	zlbSettings->insertItem ( item,-1 );
	#endif
	
	ZSoftKey *softKey  = new ZSoftKey(NULL, this, this);
	softKey->setText( ZSoftKey::LEFT, lng->getString("OK"), ( ZSoftKey::TEXT_PRIORITY )0 );
	softKey->setText( ZSoftKey::RIGHT, lng->getString("CANCEL"), ( ZSoftKey::TEXT_PRIORITY )0 );
	softKey->setClickedSlot ( ZSoftKey::LEFT, this, SLOT ( accept() ) );
	softKey->setClickedSlot ( ZSoftKey::RIGHT, this, SLOT ( reject() ) );
	setSoftKey( softKey );
	
	toLog("ZSettingDlg: end");
}

ZAppSetting::~ZAppSetting()
{
}

#ifdef AUTORUN
inline QString getFullProgramDir()
{
	QString ProgDir = lng->ProgDir;
	if ( ProgDir[0]=='.' )
	#ifndef CUTED_QT_AND_EZX
		ProgDir = QDir::current().absPath() + ProgDir.right(ProgDir.length()-1);
	#else
		ProgDir = QDir(QDir::currentDirPath()).absPath() + ProgDir.right(ProgDir.length()-1);	
	#endif
	return ProgDir;
}

inline void ZAppSetting::addToAutorun()
{
	ZConfig registry( "/ezxlocal/download/appwrite/am/InstalledDB" );
	if ( registry.readEntry(AUTORUN_APPID, "Name", "")!="zTaskManDaemon" )
	{
		registry.writeEntry(AUTORUN_APPID, "Name", "zTaskManDaemon");
		registry.writeEntry(AUTORUN_APPID, "ObjectType", 2);
		registry.writeEntry(AUTORUN_APPID, "AppID", AUTORUN_UUID);
		registry.writeEntry(AUTORUN_APPID, "AppType", 0);
		registry.writeEntry(AUTORUN_APPID, "Args", "-daemon" );
		registry.writeEntry(AUTORUN_APPID, "Attribute", 1);
		registry.writeEntry(AUTORUN_APPID, "Directory", getFullProgramDir());
		registry.writeEntry(AUTORUN_APPID, "Daemon", 1);
		registry.writeEntry(AUTORUN_APPID, "ExecId", "zTaskMan");
		registry.writeEntry(AUTORUN_APPID, "GroupID", "root");
		registry.writeEntry(AUTORUN_APPID, "UserID", "root");
		registry.writeEntry(AUTORUN_APPID, "LockEnabled", 0);	
		registry.writeEntry(AUTORUN_APPID, "Vendor", "Ant-ON");
		registry.writeEntry(AUTORUN_APPID, "Version", "");
		registry.flush();
	}
	ZConfig config("/ezxlocal/download/appwrite/am/UserConfig");
	QString sAutoRunApps = config.readEntry("AutoRun", "Apps");
	
	ZOptionItem *item = (ZOptionItem *)zlbSettings->item(0);
	
	if ( item->getNum() )
	{
		if ( sAutoRunApps.isEmpty() )
			sAutoRunApps = AUTORUN_APPID;
		else if ( sAutoRunApps.find( AUTORUN_APPID ) == -1 )
				sAutoRunApps += ";" + QString(AUTORUN_APPID);
	} else 
	{
        QStringList list = QStringList::split( ';', sAutoRunApps);
        QStringList::Iterator it = list.find(AUTORUN_APPID);
        if ( list.end() != it )
			list.remove( it );
        sAutoRunApps = list.join(";");	
	}
    config.writeEntry("AutoRun", "Apps", sAutoRunApps);
    config.flush();
}
#else
inline void ZAppSetting::addToAutorun() {};
#endif

void ZAppSetting::accept()
{
	toLog("ZSettingDlg::save");
	
	ZOptionItem *item;
	
	for ( int i=0; i<zlbSettings->count(); i++ )
	{
		item = (ZOptionItem *)zlbSettings->item(i);
		if ( !item )
			continue;
		
		switch (item->getId())
		{
		#ifdef AUTORUN
		case 0:
		{
			addToAutorun();
			break;
		}
		#endif
		case 8:
		{
			ZConfig cfg(APP_CONF_NAME);
			cfg.writeEntry("General", "Lng", lng->getLngFileName( item->getText() ));
			cfg.flush();
			
			break;
		}
		default:
			item->write();
		}
	}
	
	settings->reloadSettings();
	
	MyBaseDlg::accept();
}
