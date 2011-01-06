//
// Project: zTaskMan
//
// C++ Implementation: ZAddAppDlg
//
// Description:
//
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2008-2010
//

#include "BaseDlg.h"
#include "ZAddApp.h"
#include "zTaskMan.h"
#include "lng.h"
#include "ZDefs.h"
#include "ZLoadSettings.h"
#include "ZUtils.h"

#include <ZSoftKey.h>
#include <ZListBox.h>
#include <ZConfig.h>
#include <ZApplication.h>
#include <RES_ICON_Reader.h>

extern ZTaskMan *zTaskMan;
extern ZApplication *app;
extern ZLng* lng;
extern ZSettings * settings;

#ifdef NEW_PLATFORM
#define ICON_NAME "SimpleIcon"
#else
#define ICON_NAME "Icon"
#endif

ZAddAppDlg::ZAddAppDlg()
    :MyBaseDlg()
{
	toLog("ZAddAppDlg: Start");	
  	setMainWidgetTitle(lng->getString("DLG_ADD"));
  	
	lbAppList = new ZListBox ( QString ( "%I%M" ), this, 0);
	lbAppList->setFixedWidth ( SCREEN_WHIDTH ); 
	lbAppList->sort(true);

	setContentWidget ( lbAppList );

	ZSoftKey *softKey = new ZSoftKey ( NULL, this, this );
	softKey->setText ( ZSoftKey::LEFT, lng->getString("ADD"), ( ZSoftKey::TEXT_PRIORITY ) 0 );
	softKey->setText ( ZSoftKey::RIGHT, lng->getString("CANCEL"), ( ZSoftKey::TEXT_PRIORITY ) 0 );
	softKey->setClickedSlot ( ZSoftKey::RIGHT, this, SLOT ( reject() ) );
	softKey->setClickedSlot ( ZSoftKey::LEFT, this, SLOT ( accept() ) );
	setCSTWidget ( softKey );
	
	toLog("ZAddAppDlg: Build app list");
	
	procReg( CARD_REGISTRY );
	#ifdef OLD_PLATFORM
	procReg( USER_REGISTRY);
	#endif
	procReg( SYS_REGISTRY );
	procReg( INSTALLED_DB );

	toLog("ZAddAppDlg: End");
}

void ZAddAppDlg::procReg(QString FileRegistry)
{
	QString id;
	QString name;
	QString ico;
	static int i=0;
	
	ZConfig confReg ( FileRegistry, false );
	QStringList grouplist;
	confReg.getGroupsKeyList ( grouplist );
	for ( QStringList::Iterator it = grouplist.begin(); it != grouplist.end(); ++it ) 
	{	 
		name = confReg.readEntry ( *it, "Name", "" );
		id = confReg.readEntry ( *it, "AppID", "" );
		ico = confReg.readEntry ( *it, ICON_NAME, "" );
		#ifdef NEW_PLATFORM
		if (ico=="")
		{
			ico = confReg.readEntry ( *it, "Icon", "" );
		}		
		#endif		
		if (ico=="")
		{
			ico = confReg.readEntry ( *it, "BigIcon", "" );
		}		
		if ( (id!="")&&(name!=""))
		{
			AppRegistry app;
			app.setId(id);
			app.setName(name);
			#ifndef NEW_PLATFORM
			if ( ico.find(".g") == -1 && !QFile::exists(ico) )
			#else
			if ( ico.find(".g") == -1 && ico.find(".k") == -1 &&!QFile::exists(ico) )
			#endif
			{
					ico = confReg.readEntry ( *it, "Directory", "" ) + "/" + ico;
			}	
			app.setIcon(ico);
			mapApp[ i ] = app;
			addApp(name, ico, i);
			++i;
		}
	}	
}

void ZAddAppDlg::addApp(QString name, QString icon, int i)
{
	#ifdef FIX_LISTBOX
	ZListBoxItem* listitem = new ZListBoxItem( lbAppList, QString ( "%I%M" ) );
	#else
	ZSettingItem* listitem = new ZSettingItem( lbAppList, QString ( "%I%M" ) );	
	#endif
	listitem->appendSubItem ( 1, name , true );
	QPixmap pm;
	
	#ifndef NEW_PLATFORM
	if (icon.right(2)==".g")
	#else
	if ( (icon.right(2)==".g") || (icon.right(2)==".k") )
	#endif
	{
		RES_ICON_Reader res;
		pm = res.getIcon(icon);
	} else
	{
		if ( !QFile::exists(icon) )		
			icon = settings->getProgramDir() + QString ( "/img/run_app.png" );
		pm.load( icon );
	}
	imgSetMaxSize(&pm, 19);	
	listitem->setPixmap ( 0, pm );
	listitem->setReservedData ( i );
	lbAppList->insertItem ( listitem,-1,true );
}

void ZAddAppDlg::accept()
{
	toLog("ZAddAppDlg: OK: Add app");
	
	int n = lbAppList->currentItem();
	
	if (n == -1) 
	{
		return;
	}
	
	#ifdef FIX_LISTBOX
	ZListBoxItem* listitem = lbAppList->item ( n );
	#else
	ZSettingItem* listitem = (ZSettingItem*)lbAppList->item ( n ); 
	#endif
	int id = listitem->getReservedData();
	
	ZConfig cfg(appConf);
	
	for (int i=0;i<20;i++)
	{
		QString name = cfg.readEntry(QString("App"), QString("name"+QString::number(i)), "");
		if (name == "")
		{
			AppRegistry it = mapApp[id];
			cfg.writeEntry("App", "name"+QString::number(i), it.name() );
			cfg.writeEntry("App", "run"+QString::number(i), it.id() );
			cfg.writeEntry("App", "icon"+QString::number(i), it.icon() );
			break;
		}
	}
	 
	cfg.flush();
	
	toLog("ZAddAppDlg: OK:  End");
	
	MyBaseDlg::accept();
}

void ZAddAppDlg::reject()
{
	MyBaseDlg::reject();
} 

ZAddAppDlg::~ZAddAppDlg()
{

}
