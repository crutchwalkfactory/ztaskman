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

#include <ZSoftKey.h>
#include <ZFormContainer.h>
#include <ZConfig.h>
#include <ZSeparator.h>
#include <ZComboBox.h>

#include <qdir.h>

extern ZTaskMan *zTaskMan;
extern ZLng* lng;
extern ZSettings* settings;

ZAppSetting::ZAppSetting()
{
	toLog("ZSettingDlg: Start");
	
	setMainWidgetTitle(lng->getString("DLG_SETTING"));
	
	ZFormContainer *form = new ZFormContainer(this, 0, ZSkinService::clsZFormContainer);
	this->setContentWidget(form);

	toLog("ZSettingDlg: Create CheckBox and Load settings");
	
	ZConfig cfg(appConf);

	#ifdef AUTORUN
	zcbDaemon = new ZCheckBox("Daemon",form);
	zcbDaemon->setChecked(false);
	form->addChild(zcbDaemon);

	ZConfig config("/ezxlocal/download/appwrite/am/UserConfig");
	QString sAutoRunApps = config.readEntry("AutoRun", "Apps");
	QStringList list = QStringList::split( ';', sAutoRunApps);
	QStringList::Iterator it = list.find(AUTORUN_APPID);
	if ( list.end() != it )
		zcbDaemon->setChecked(true);
	#endif

	zcbFiltrProc = new ZCheckBox(lng->getString("PROCFILTER"),form);
	zcbTaskNoWin = new ZCheckBox(lng->getString("TASKNOWIN"),form);
	#ifdef RAISE_PHONE
	zcbSendReasePhone = new ZCheckBox(lng->getString("SENDREASEPHONE"),form);
	zcbSendGoToIDLE = new ZCheckBox(lng->getString("SENDTOIDLE"),form);
	#endif
    zcbKeyGrren = new ZCheckBox(lng->getString("SHOWINFOGREEN"),form);

	zcbFiltrProc->setChecked(!settings->cfg_FiltrProc);
	zcbTaskNoWin->setChecked(settings->cfg_ShowAppNoWindow);
	#ifdef RAISE_PHONE
	zcbSendReasePhone->setChecked(settings->cfg_SendReaisePhone);
	zcbSendGoToIDLE->setChecked(settings->cfg_SendGoToIDLE);
	#endif
    zcbKeyGrren->setChecked(settings->cfg_GreenShowInfo);
    
	zcbActionC = new ZComboBox(false, form);
	zcbActionC->setTitle(lng->getString("ACTIONC"));
    zcbActionC->insertItem(lng->getString("NO"));
	zcbActionC->insertItem(lng->getString("KILL"));
	zcbActionC->insertItem(lng->getString("QUIT"));
	zcbActionC->setCurrentItem(settings->cfg_CAction);

	zcbLanguage = new ZComboBox(false, form);
	zcbLanguage->setTitle(lng->getString("LANG"));
	lng->getLngList(zcbLanguage, lng->getCurrentLng());	

	fontSize = settings->cfg_ListFontSize;
	zpbFontSize = new ZPressButton( form );
	zpbFontSize->setTitle( lng->getString("LISTFONTSIZE") );
	zpbFontSize->setText(QString::number(fontSize));
	connect ( zpbFontSize, SIGNAL ( clicked() ), this, SLOT ( changeFont() ) );

	fontPanelSize = settings->cfg_PanelFontSize;
	zpbPanelFontSize = new ZPressButton( form );
	zpbPanelFontSize->setTitle( lng->getString("PANELFONTSIZE") );
	zpbPanelFontSize->setText(QString::number(fontPanelSize));
	connect ( zpbPanelFontSize, SIGNAL ( clicked() ), this, SLOT ( changePanelFont() ) );	
	
	zcbCustomFontColor = new ZCheckBox(lng->getString("USEUSERCOLOR"),form);
	zcbCustomFontColor->setChecked(settings->cfg_UserFont);
	
	#if defined(EZX_ZN5) || defined(EZX_U9) || defined(EZX_Z6W)
	fontColor = settings->cfg_FontColor;
	zpbFontColor = new ZPressButton( form );
	zpbFontColor->setTitle( lng->getString("COLOR") );
	zpbFontColor->setText("###");
	zpbFontColor->setFontColor(fontColor);
	connect ( zpbFontColor, SIGNAL ( clicked() ), this, SLOT ( changeFontColor() ) );		
	#else
	zccFontColor = new ZColorControl( settings->cfg_FontColor );
	zccFontColor->setCaption( lng->getString("COLOR") );
	#endif
	
	zcbTimeInCaption = new ZCheckBox(lng->getString("TIMEINCAPTION"),form);
	zcbTimeInCaption->setChecked(settings->cfg_TimeInCaption);	
	
	toLog("ZSettingDlg: init interfase");

	form->addChild(zcbFiltrProc);
	form->addChild(zcbTaskNoWin);
	form->addChild(zcbTimeInCaption);
	ZSeparator* sep = new ZSeparator();
	#ifdef RAISE_PHONE
	form->addChild(sep);
	form->addChild(zcbSendReasePhone);
	form->addChild(zcbSendGoToIDLE);
	sep = new ZSeparator();
	#endif
	form->addChild(sep);
	form->addChild(zcbKeyGrren);
	form->addChild(zcbActionC);
	sep = new ZSeparator();
	form->addChild(sep);	
	form->addChild(zcbLanguage);

	sep = new ZSeparator();
	form->addChild(sep);

	#ifdef FIX_FORMCONTAINER
	form->addChild( (ZWidget*)zpbFontSize );
	form->addChild( (ZWidget*)zpbPanelFontSize );
	#else
	form->addChild( zpbFontSize );
	form->addChild( zpbPanelFontSize );	
	#endif
	form->addChild( zcbCustomFontColor );
	#ifdef CUTED_QT_AND_EZX
	form->addChild( zpbFontColor );	
	#else
	form->addChild( zccFontColor );		
	#endif
	
	ZSoftKey *softKey  = new ZSoftKey(NULL, this, this);
	setMainWidgetTitle(lng->getString("DLG_SETTING"));
	softKey->setText ( ZSoftKey::LEFT, lng->getString("OK"), ( ZSoftKey::TEXT_PRIORITY ) 0 );
	softKey->setText ( ZSoftKey::RIGHT, lng->getString("CANCEL"), ( ZSoftKey::TEXT_PRIORITY ) 0 );
	softKey->setClickedSlot ( ZSoftKey::RIGHT, this, SLOT ( reject() ) );
	softKey->setClickedSlot ( ZSoftKey::LEFT, this, SLOT ( accept() ) );
	setSoftKey( softKey );
	
	toLog("ZSettingDlg: end");
}

ZAppSetting::~ZAppSetting()
{
}

void ZAppSetting::accept()
{
	toLog("ZSettingDlg::save");
	
	#ifdef RAISE_PHONE
	settings->cfg_SendReaisePhone = zcbSendReasePhone->isChecked();
	settings->cfg_SendGoToIDLE = zcbSendGoToIDLE->isChecked();
	#endif
	settings->cfg_FiltrProc = !zcbFiltrProc->isChecked();
	settings->cfg_ShowAppNoWindow = zcbTaskNoWin->isChecked();
	settings->cfg_PanelFontSize = fontPanelSize;
	settings->cfg_ListFontSize = fontSize; 
	settings->cfg_Language = lng->getLngFileName( zcbLanguage->currentText() );
    settings->cfg_GreenShowInfo = zcbKeyGrren->isChecked();
    settings->cfg_CAction = zcbActionC->currentItem();
	
	settings->cfg_UserFont = zcbCustomFontColor->isChecked();
	settings->cfg_TimeInCaption = zcbTimeInCaption->isChecked();
	#if CUTED_QT
	settings->cfg_FontColor = fontColor;
	#else
	settings->cfg_FontColor = zccFontColor->getColor();
	#endif
	
	#ifdef AUTORUN
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
	if ( zcbDaemon->isChecked() )
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
	#endif
	
	//write settings
	settings->save();

	MyBaseDlg::accept();
}

void ZAppSetting::reject()
{
	MyBaseDlg::reject();
} 

void ZAppSetting::changeFont()
{
	ZNumPickerDlg * dlg = new ZNumPickerDlg(2,this);
	ZNumModule* num = dlg->getNumModule();
	num->setMaxValue(25);
	num->setMinValue(6);
	num->setValue(fontSize);
	if ( dlg->exec() == QDialog::Accepted )
	{
		fontSize = num->getValue();
		zpbFontSize->setText( QString::number(fontSize) );
	}
	delete num;
	delete dlg;
}

QString ZAppSetting::getFullProgramDir()
{
	QString ProgDir = lng->ProgDir;
	if ( ProgDir[0]=='.' )
		ProgDir = QDir::current().absPath() + ProgDir.right(ProgDir.length()-1);
	return ProgDir;
}

void ZAppSetting::changePanelFont()
{
	ZNumPickerDlg * dlg = new ZNumPickerDlg(2,this);
	ZNumModule* num = dlg->getNumModule();
	num->setMaxValue(30);
	num->setMinValue(6);
	num->setValue(fontPanelSize);
	if ( dlg->exec() == QDialog::Accepted )
	{
		fontPanelSize = num->getValue();
		zpbPanelFontSize->setText(QString::number(fontPanelSize));
	}
	delete num;
	delete dlg;
}

#ifdef CUTED_QT
void ZAppSetting::changeFontColor()
{
	ZColorPickerDlg * dlg = new ZColorPickerDlg(fontColor,NULL,0,this);
	if ( dlg->exec() == QDialog::Accepted )
	{
		fontColor = dlg->getColor();;
		zpbFontColor->setFontColor(fontColor);
	}
	delete dlg;
}
#else
void ZAppSetting::changeFontColor(){};
#endif
