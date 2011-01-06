//
// Project: zTaskMan
//
// C++ Implementation: ZProcInfoDlg
//
// Description: process information dlg
//
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2008-2011
//

#include "BaseDlg.h"
#include "ZProcInfo.h"
#include "zTaskMan.h"
#include "ZDefs.h"
#include "lng.h"
#include "ZLoadSettings.h"
#include "ZUtils.h"

#include <ZLabel.h>
#include <ZApplication.h>
#include <ZSoftKey.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qfont.h>
#include <ZSkinBase.h>

#include <sys/time.h>
#include <sys/resource.h>

extern ZTaskMan *zTaskMan;
extern ZApplication *app;
extern ZLng* lng;
extern ZSettings * settings;

ZProcInfoDlg::ZProcInfoDlg(int pid)
    :MyBaseDlg()
{
	toLog("ZProcInfoDlg: Start");
	QWidget *myWidget = new QWidget ( this ); 
  	
  	setMainWidgetTitle(lng->getString("DLG_PROCINFO"));
  	
  	toLog("ZProcInfoDlg: Get Cmd line");
	char *cmd;
	char buf[1024];	
	cmd =0;
	FILE *fp;
	char *name;
	int n;
	
	fp = fopen(QCString("/proc/"+QString::number(pid)+"/cmdline"), "r") ;
	if((n=fread(buf, 1, sizeof(buf)-1, fp)) > 0) 
	{
		if(buf[n-1]=='\n')
			buf[--n] = 0;
		name = buf;
		while(n) 
		{
			if(((unsigned char)*name) < ' ')
				*name = ' ';
			name++;
			n--;
		}
		*name = 0;
		if(buf[0])
			cmd = strdup(buf);
	}
	fclose(fp);

	toLog("ZProcInfoDlg: Add visual component");

	tabWidget = new ZNavTabWidget(this);

	QPixmap imgTab1,imgTab2;
	imgTab1.load(settings->getProgramDir() + "/img/info.png"  );
	imgTab2.load(settings->getProgramDir() + "/img/liblist.png"  );

	form = new ZFormContainer(this, 0, ZSkinService::clsZFormContainer);

	QFont font ( qApp->font() ); 

	ZLabel * myLabel = new ZLabel ( QString ( zTaskMan->extractExecName(QString(cmd))), myWidget );
	myLabel->setAutoResize(true);
	font.setPointSize ( 20 );
	myLabel->setFont(font);
	myLabel->setFixedWidth ( SCREEN_WHIDTH );
	myLabel->setAlignment(ZLabel::AlignHCenter);
	form->addChild( myLabel );

	font.setPointSize ( 14 );
	
	myLabel = new ZLabel ( QString ( "Pid: "+QString::number(pid) ), myWidget );
	myLabel->setAutoResize(true);
	myLabel->setFont(font);
	myLabel->setFixedWidth ( SCREEN_WHIDTH ); 
	form->addChild( myLabel );

	myLabel = new ZLabel ( QString ( "Cmd: "+QString(cmd) ), myWidget );
	myLabel->setAutoResize(true);
	myLabel->setFont(font);
	myLabel->setLeading(14);
	myLabel->setFixedWidth ( SCREEN_WHIDTH - 5 ); 
	form->addChild( myLabel );

	myLabel = new ZLabel ( getStatus(pid, "VmSize:"), myWidget );
	myLabel->setAutoResize(true);
	myLabel->setFont(font);
	myLabel->setFixedWidth ( SCREEN_WHIDTH ); 
	form->addChild( myLabel );
	
	myLabel = new ZLabel ( getStatus(pid, "State:"), myWidget );
	myLabel->setAutoResize(true);
	myLabel->setFont(font);
	myLabel->setFixedWidth ( SCREEN_WHIDTH ); 
	form->addChild( myLabel );
	
	myLabel = new ZLabel ( lng->getString("THREADS") + ": " + getStatus(pid, "Threads:", false), myWidget );
	myLabel->setAutoResize(true);
	myLabel->setFont(font);
	myLabel->setFixedWidth ( SCREEN_WHIDTH ); 
	form->addChild( myLabel );	
	
	myLabel = new ZLabel ( lng->getString("RUNUSER") + ": " + (getStatus(pid, "Uid:", false)=="0	0	0	0"?"root":"EZX"), myWidget );
	myLabel->setAutoResize(true);
	myLabel->setFont(font);
	myLabel->setFixedWidth ( SCREEN_WHIDTH ); 
	form->addChild( myLabel );	
	
	myLabel = new ZLabel ( lng->getString("PRIORITY") + ": "+QString::number( getpriority(PRIO_PROCESS, pid) ), myWidget );
	myLabel->setAutoResize(true);
	myLabel->setFont(font);
	myLabel->setFixedWidth ( SCREEN_WHIDTH ); 
	form->addChild( myLabel );			
	
	usesLib = new ZListBox ( QString ( "%I%M" ), this, 0);
	usesLib->setFixedWidth ( SCREEN_WHIDTH );
	font.setPointSize ( 12 );
	font.setBold ( true );
	
	usesLib->setItemFont (ZListBox::LISTITEM_REGION_C, font, true );
	usesLib->setItemFont (ZListBox::LISTITEM_REGION_C, font, false );	
	
	toLog("ZProcInfoDlg: Get loadded lib");
    QFile f("/proc/"+QString::number(pid)+"/maps");
    QString s;
    QString htm;

    if ( f.open( IO_ReadOnly ) )
    {
        QTextStream t( &f );

        while ( !t.eof() )
		{
            s = t.readLine();
            if ( s.find("/SYSV")==-1 )
            {
           		s.remove(0, s.find("/"));
           		s.stripWhiteSpace();
           		if ( htm.find(s)==-1)
           		{ 
           			htm=htm+s+"|";
           			ZListBoxItem* listitem = new ZListBoxItem ( usesLib );
					listitem->appendSubItem ( 1, QString( s ) , true, NULL );
					usesLib->insertItem ( listitem,-1,true );      			
				}
			}
        }

        f.close();
    }
    
    tabWidget->addTab(form, QIconSet(imgTab1), "");	
    tabWidget->addTab(usesLib, QIconSet(imgTab2), "");

	setContentWidget(tabWidget);

	connect(tabWidget,SIGNAL(currentChanged(QWidget* )),this,SLOT(slotPageChanged(QWidget* )));	
	
	ZSoftKey *softKey = new ZSoftKey ( NULL, this, this );
	softKey->setText ( ZSoftKey::LEFT, lng->getString("OK"), ( ZSoftKey::TEXT_PRIORITY ) 0 );
	softKey->setClickedSlot ( ZSoftKey::LEFT, this, SLOT ( accept() ) );
	setCSTWidget ( softKey );
	
	this->installEventFilter( this );
	
	toLog("ZProcInfoDlg: End");
}

ZProcInfoDlg::~ZProcInfoDlg()
{
	this->removeEventFilter( this );
}

QString ZProcInfoDlg::getStatus(int pid, QString param, bool showName)
{
    QFile f("/proc/"+QString::number(pid)+"/status");
    QString s;
    if ( f.open( IO_ReadOnly ) )
    {
        QTextStream t( &f );

        while ( !t.eof() )
		{
            s = t.readLine();
            if ( s.find(param)>-1)
            {
				if (showName)
				{
	            	while ( s.find("  ")>-1 )
						s.remove( s.find("  ")+1, 1 );
					s=s.simplifyWhiteSpace();	
				} else
				{
					s.remove( 0, s.find(":")+1 );	
				}
	           	s=s.stripWhiteSpace();
	           	return s;
			}
        }

        f.close();
    }
    if (showName)
		return param;
	return "";	
}

void ZProcInfoDlg::slotPageChanged(QWidget*)
{
	int i = tabWidget->currentPageIndex();
	
	switch (i)
	{
		case 0:
		{
			form->setFocus();
			break; 
		}
		case 1:
		{
			usesLib->setFocus();
			break; 
		}
	}
}

bool ZProcInfoDlg::eventFilter(QObject* o, QEvent* pEvent)
{
	if ( tabWidget->currentPageIndex() == 0 ) //For fix not scroll ZFormContainer on one tab
	{
		if ( QEvent::KeyRelease == pEvent->type() )
		{
			QKeyEvent *pKey = (QKeyEvent*)pEvent;
			if (pKey->key() == KEY_UP)	
			{
				form->scrollBy(0,-20);
			}
			if (pKey->key() == KEY_DOWN)	
			{
				form->scrollBy(0,20);	
			}
		}
	}
    return QWidget::eventFilter( o, pEvent ); 
}

