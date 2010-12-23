//
// C++ Implementation: ZAboutDlg
//
// Description:
//
//
// Author: root <root@andLinux>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "BaseDlg.h"
#include "ZAboutDlg.h"
#include "zTaskMan.h"
#include "ZDefs.h"
#include "lng.h"

#include <qlabel.h>
#include <qfile.h>
#include <ZApplication.h>
#include <ZSoftKey.h>
#include <ZLabel.h>
#include <ZImage.h>
#include <ZScrollPanel.h>
#include <ZSeparator.h>
#include "ZLoadSettings.h"

extern ZTaskMan *zTaskMan;
extern ZApplication *app;
extern ZLng* lng;
extern ZSettings * settings;

ZAboutDialog::ZAboutDialog()
    :MyBaseDlg()
{
	setMainWidgetTitle(lng->getString("DLG_ABOUT"));
	
	ZScrollPanel * sv = new ZScrollPanel(this);
	this->setContentWidget ( sv );  
	
	ZImage *img = new ZImage( this );
	QFile f;
	QString iconName = settings->getProgramDir() + "/zTaskMan3_usr.png";
    if ( !f.exists(iconName) ) iconName = settings->getProgramDir() + "/zTaskMan.png";
	img->setPixmap ( QPixmap ( iconName ) );
	sv->addChild(img, 10,10);

	QFont font ( qApp->font() ); 
	font.setBold(true);
	ZLabel * a = new ZLabel("zTaskMan",this);
	a->setFont( font );
	a->setFixedWidth( SCREEN_WHIDTH-70 );
	a->setAlignment( ZLabel::AlignHCenter );
	#ifdef EZX_E8
	sv->addChild(a, 76,5);	
	#else
	sv->addChild(a, 76,10);
	#endif
	
	a = new ZLabel(QString("ver. ")+LNG_VER,this);
	a->setFixedWidth( SCREEN_WHIDTH-76 );
	a->setAlignment( ZLabel::AlignHCenter );	
	#ifdef EZX_E8
	sv->addChild(a, 76,28);	
	#else
	sv->addChild(a, 76,35);
	#endif		

	#ifdef EZX_E2	
	a = new ZLabel(QString("Platform: EZX-E2"),this);
	#else
	#ifdef EZX_V8	
	a = new ZLabel(QString("Platform: EZX-V8"),this);
	#else
	#ifdef EZX_Z6	
	a = new ZLabel(QString("Platform: EZX-Z6"),this);
	#else
	#ifdef EZX_Z6W	
	a = new ZLabel(QString("Platform: EZX-Z6w"),this);
	#else	
	#ifdef EZX_ZN5	
	a = new ZLabel(QString("Platform: EZX-ZN5"),this);
	#else
	#ifdef EZX_U9	
	a = new ZLabel(QString("Platform: EZX-U9"),this);
	#else
	#ifdef EZX_EM30	
	a = new ZLabel(QString("Platform: EZX-EM30"),this);
	#else
	#ifdef EZX_EM35	
	a = new ZLabel(QString("Platform: EZX-EM35"),this);
	#else
	#ifdef EZX_VE66	
	a = new ZLabel(QString("Platform: EZX-VE66"),this);
	#else	
	#ifdef EZX_E8	
	a = new ZLabel(QString("Platform: EZX-E8"),this);
	#endif		
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif	
	#endif
	#endif
	#endif		
		
	a->setFixedWidth( SCREEN_WHIDTH-70 );
	a->setAlignment( ZLabel::AlignHCenter );	
	#ifdef EZX_E8
	sv->addChild(a, 76,52);	
	#else
	sv->addChild(a, 76,60);
	#endif	
	
	ZSeparator * sep = new ZSeparator();
	sep->setLength(SCREEN_WHIDTH-20);
	#ifdef EZX_E8
	sv->addChild(sep, 10,80);	
	#else
	sv->addChild(sep, 10,100);
	#endif	
	
	a = new ZLabel("Copyrigth ©: Ant-ON, 2010",this);
	a->setAutoResize(true);
	a->setPreferredWidth(SCREEN_WHIDTH-5);
	#ifdef EZX_E8
	sv->addChild(a, 10,85);
	#else
	sv->addChild(a, 10,110);
	#endif		
	
	a = new ZLabel("e-mail: prozanton@gmail.com",this);
	a->setAutoResize(true);
	a->setPreferredWidth(SCREEN_WHIDTH-5);
	#ifdef EZX_E8
	sv->addChild(a, 10,107);
	#else
	sv->addChild(a, 10,140);
	#endif	
	
	a = new ZLabel(LNG_THANKS,this);
	a->setAutoResize(true);
	a->setPreferredWidth(SCREEN_WHIDTH-5);
	#ifdef EZX_E8
	sv->addChild(a, 10,129);
	#else
	sv->addChild(a, 10,170);
	#endif	
	
	a = new ZLabel("Based on zSimpleGUI by BeZ",this);
	a->setAutoResize(true);
	a->setPreferredWidth(SCREEN_WHIDTH-5);
	#ifdef EZX_E8
	sv->addChild(a, 10,150);
	#else
	sv->addChild(a, 10,200);
	#endif
	
	sep = new ZSeparator();
	sep->setLength(SCREEN_WHIDTH-20);
	#ifdef EZX_E8
	sv->addChild(sep, 10,173);	
	#else
	sv->addChild(sep, 10,230);
	#endif

	ZSoftKey *softKey = new ZSoftKey ( NULL, this, this );
	softKey->setText ( ZSoftKey::LEFT, lng->getString("OK"), ( ZSoftKey::TEXT_PRIORITY ) 0 );
	softKey->setClickedSlot ( ZSoftKey::LEFT, this, SLOT ( accept() ) );
	#ifndef EZX_V8
	setSoftKey( softKey );
	#else
	setCSTWidget( softKey );
	#endif
}

ZAboutDialog::~ZAboutDialog()
{

}
