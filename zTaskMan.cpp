//
// Project: zTaskMan
//
// C++ Implementation: ZTaskMan
//
// Description: task manager main window class
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2008-2011
//
// Based on zSimpleGUI by BeZ
//

//My
#include "zTaskMan.h"
#include "GUI_Define.h"
#include "BaseDlg.h"
#include "ZAboutDlg.h"
#include "ZProcInfo.h"
#include "ZSettingDlg.h"
#include "ZAddApp.h"
#include "ZTaskItem.h"
#include "lng.h"
#include "ZLoadSettings.h"
#include "ZNumSelect.h"
#include "ZUtils.h"

// C/C++
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

//EZX
#include <ZListBox.h>
#include <ZSoftKey.h>
#include <ZOptionsMenu.h>
#include <ZMessageDlg.h>
#include <ZSingleCaptureDlg.h>
#include <ZConfig.h>
#include <RES_ICON_Reader.h>

//QT
#include <qmap.h>
#include <qfile.h>
#include <qcopchannel_qws.h>
#include <qpixmap.h>
#include <qregexp.h>
#include <qevent.h>
#include <qtextstream.h>

#ifndef RAISE_PHONE
#include <SETUP_UTIL.h>
#endif

#ifndef FIX_HEADER
#define just_ok TypeOK
#define yes_no TypeChoose
#define FULL_TYPE MAINDISPLAY_HEADER
#endif

#if FIX_MENU_2
#define MENU_FIX QString("   ")
#else
#define MENU_FIX ""
#endif

extern ZLng* lng;
extern ZSettings * settings;
extern bool isDaemon;

ZTaskMan::ZTaskMan ( QWidget* parent, const char*, WFlags )
    : ZKbMainWidget ( ZHeader::FULL_TYPE, NULL, "ZMainWidget", WType_TopLevel )
{	
	toLog("Check run SDL app");
	if ( QCopChannel::isRegistered(SDL_LIB_CHENEL) )
	{
		QCopChannel::send( SDL_LIB_CHENEL, "hide()" );
		toLog("Send hide SDL lib");
	}
  
    timer = NULL;
  
	#ifndef NO_SHOW_GRAPH
	//Init proc and mem graph
	memset(proc_graph, 0, GRAPH_LEN*sizeof(int));
	memset(swap_graph, 0, GRAPH_LEN*sizeof(int));
	memset(mem_graph, 0, GRAPH_LEN*sizeof(int));
	graph_old=0;
	upGraph=false;
	#endif
  
	CreateWindow ( parent );
	qApp->setMainWidget( this );
	this->show();
	lbProc->setGeometry( 0, 0, SCREEN_WHIDTH-PANEL_WHIDTH, sv->height());
	sp->setGeometry( SCREEN_WHIDTH-PANEL_WHIDTH, 0, PANEL_WHIDTH, sv->height());
}

ZTaskMan::~ZTaskMan()
{

}

void ZTaskMan::CreateWindow ( QWidget* )
{
	qApp->installEventFilter( this );
      	
	//************************** interface *****************************	
	toLog(QString("CreateWindow: interface"));
	setMainWidgetTitle ( "zTaskMan" );

	tabWidget = new ZNavTabWidget(this);

	QPixmap imgTab1,imgTab2,imgTab3;
	imgTab1.load(settings->getProgramDir() + "/img/tab_task.png"  );
	imgTab2.load(settings->getProgramDir() + "/img/tab_favorits.png"  );
	imgTab3.load(settings->getProgramDir() + "/img/tab_graph.png"  );
  
	#ifdef OLD_PLATFORM
	sv = new ZScrollView(this, 0, 0);
	#else
	sv = new QScrollView(this, 0, 0);
	#endif	
	sv->setFixedWidth ( SCREEN_WHIDTH );
  
	lbProc = new ZListBox ( QString ( "%16I%"+QString::number(SCREEN_WHIDTH - 26)+"M%20M" ), this, 0);
	lbProc->setFixedWidth ( SCREEN_WHIDTH-PANEL_WHIDTH );
	
	sp = new ZScrollPanel();
	sp->setFixedWidth( PANEL_WHIDTH );

	QFont font ( qApp->font() ); 
	font.setPointSize ( settings->cfg_PanelFontSize );
	
	WIDGET_PADDING_INFO_T wNoPading; 
	memset(&wNoPading, 0, sizeof(wNoPading));
	
	ZImage * zImg = new ZImage(sp);
	QPixmap img;
	img.load( settings->getProgramDir() + "/panel/cpu.png" );
	zImg->setPixmap(img);
	sp->addChild(zImg, 0, 0);
	
	CPUFreq = new ZLabel("", sp, "ZLabel");
	CPUFreq->setFont( font );
	CPUFreq->setAlignment( ZLabel::AlignHCenter );
	CPUFreq->setFixedWidth(PANEL_WHIDTH);
	CPUFreq->setNumLines(2);
	CPUFreq->setLeading(1);
	CPUFreq->setPadding(wNoPading);
	sp->addChild(CPUFreq, 0, zImg->y()+zImg->height()+1);
	
	imgCPU = new ZImage(sp);
	img.load(settings->getProgramDir() + "/panel/progressv.png"  );
	imgCPU->setPixmap(img);
	sp->addChild(imgCPU, zImg->x()+zImg->width()-imgCPU->width(), zImg->y());

	zImg = new ZImage(sp);
	img.load(settings->getProgramDir() + "/panel/ram.png"  );
	zImg->setPixmap(img);
	sp->addChild(zImg, 0, 80);

 	
	RAM = new ZLabel("", sp, "ZLabel");
	RAM->setFixedWidth( PANEL_WHIDTH );
	RAM->setFont( font );
	RAM->setAlignment( ZLabel::AlignHCenter );
	RAM->setPadding(wNoPading);
	sp->addChild(RAM, 0, zImg->y()+zImg->height()+1);
	
	imgMEM = new ZImage(sp);
	img.load(settings->getProgramDir() + "/panel/progress.png"  );
	imgMEM->setPixmap(img);
	imgMEM->setFixedWidth(1);
	sp->addChild(imgMEM, zImg->x(), zImg->y()+zImg->height()-imgMEM->height());
	
	zImg = new ZImage(sp);
	img.load(settings->getProgramDir() + "/panel/swap.png"  );
	zImg->setPixmap(img);
	#ifndef GORIZONTAL_SCREEN
	sp->addChild(zImg, 0, 155);
	#else
	sp->addChild(zImg, 0, 118);
	#endif

	SWAP = new ZLabel("", sp, "ZLabel");
	SWAP->setFixedWidth( PANEL_WHIDTH );
	SWAP->setFont( font );
	SWAP->setAlignment( ZLabel::AlignHCenter );
	SWAP->setPadding(wNoPading);
	sp->addChild(SWAP, 0, zImg->y()+zImg->height()+1);	
	
	imgSWAP = new ZImage(sp);
	img.load(settings->getProgramDir() + "/panel/progress.png"  );
	imgSWAP->setPixmap(img);
	imgSWAP->setFixedWidth(1);
	sp->addChild(imgSWAP, zImg->x(), zImg->y()+zImg->height()-imgSWAP->height());
	
	lbApp = new ZListBox ( QString ( "%I%M" ), this, 0);
	lbApp->setFixedWidth ( SCREEN_WHIDTH ); 
	
	#ifdef OLD_PLATFORM
	sv->setMargins(0,0,0,0);
	#else
	sv->setMargin(-2);
	#endif
	
	font.setPointSize ( settings->cfg_ListFontSize );
	
	lbProc->setItemFont( ZListBox::LISTITEM_REGION_A, ZListBox::StStandard, font );	
	lbProc->setItemFont( ZListBox::LISTITEM_REGION_A, ZListBox::StHighlighted, font );	
	lbApp->setItemFont( ZListBox::LISTITEM_REGION_A, ZListBox::StStandard, font );	
	lbApp->setItemFont( ZListBox::LISTITEM_REGION_A, ZListBox::StHighlighted, font );	

	if ( settings->cfg_UserFont )
	{	
		CPUFreq->setFontColor(settings->cfg_FontColor);
		RAM->setFontColor(settings->cfg_FontColor);
		SWAP->setFontColor(settings->cfg_FontColor);
	}

	tabWidget->stopNextWhenKeyRepeat(true);
	tabWidget->addTab(sv, QIconSet(imgTab1), "");
	
	setContentWidget ( tabWidget );
	
	sv->addChild(lbProc, 0, 0);
	sv->addChild(sp, SCREEN_WHIDTH-PANEL_WHIDTH, 0);
	
	tabWidget->addTab(lbApp, QIconSet(imgTab2), "");
	
    #ifndef NO_SHOW_GRAPH
	gp = new ZScrollPanel();
	ZLabel * label = new ZLabel("CPU", gp, "ZLabel");
	label->setFont( font );
    imgGraphProc = new ZImage(gp);
    QPixmap * pix = new QPixmap(GRAPH_WIDTH,GRAPH_HEIGHT);
    imgGraphProc->setPixmap(*pix);
	gp->addChild(label, 12, 0);    
    gp->addChild(imgGraphProc, 10, 20);
    
	label = new ZLabel("RAM", gp, "ZLabel");
	label->setFont( font );
	imgGraphMem = new ZImage(gp);
	imgGraphMem->setPixmap(*pix);
	gp->addChild(label, 12, 20+GRAPH_HEIGHT);   	
	gp->addChild(imgGraphMem, 10, 20+GRAPH_HEIGHT+20);
	
	label = new ZLabel("SWAP", gp, "ZLabel");
	label->setFont( font );
	imgGraphSwap = new ZImage(gp);
	imgGraphSwap->setPixmap(*pix);
	#ifndef GORIZONTAL_SCREEN
	gp->addChild(label, 12, 20+GRAPH_HEIGHT+20+GRAPH_HEIGHT);	
	gp->addChild(imgGraphSwap, 10, 20+GRAPH_HEIGHT+20+GRAPH_HEIGHT+20);
	#else
	gp->addChild(label, 10+GRAPH_WIDTH+12, 0);	
	gp->addChild(imgGraphSwap, 10+GRAPH_WIDTH+10, 20);	
	#endif
	tabWidget->addTab(gp, QIconSet(imgTab3), "");
	#endif
	
	//*********************** softkeys and menus ***********************
	toLog("CreateWindow: softkeys and menus");
	
	softKey = new ZSoftKey("CST_2", this, this);

	QRect rect(0, 0, SCREEN_WHIDTH, SCREEN_HEIGHT);
	#ifdef FIX_MENU_1
	menu = new ZOptionsMenu ( rect, softKey, NULL, 0, ZSkinService::WidgetClsID(55)); 
	#else 
	menu = new ZOptionsMenu ( rect, softKey, NULL, 0); 
	#endif
	menu->setItemSpacing(10);
  	
  	createProcMenu();
  	
	softKey->setOptMenu ( ZSoftKey::LEFT, menu );
	softKey->setClickedSlot ( ZSoftKey::RIGHT, this, SLOT ( Quit() ) );
	softKey->setText ( ZSoftKey::LEFT, lng->getString("MENU"), ( ZSoftKey::TEXT_PRIORITY ) 0 );//Menu
	softKey->setTextForOptMenuHide(lng->getString("MENU"));
	softKey->setTextForOptMenuShow(lng->getString("SEL"), lng->getString("CANCEL"));	
	softKey->setText ( ZSoftKey::RIGHT, lng->getString("EXIT"), ( ZSoftKey::TEXT_PRIORITY ) 0 );//Exit
	
	setSoftKey(softKey);
		
	//**************************** SIGNAL ******************************
	toLog("CreateWindow: SIGNAL");
	connect ( lbApp, SIGNAL ( selected ( int ) ), this, SLOT ( lbAppSel ( int ) ) );
	connect ( lbProc, SIGNAL ( selected ( int ) ), this, SLOT ( lbProcSel ( int ) ) );
	connect( qApp, SIGNAL(signalRaise()), this, SLOT(slotRaise()) );
	connect( qApp, SIGNAL(askReturnToIdle(int)), this, SLOT(slotReturnToIdle(int)) );
	connect(tabWidget,SIGNAL(currentChanged(QWidget* )),this,SLOT(slotPageChanged(QWidget* )));	
	
	showList();
	showInfo();
	
	toLog(QString("CreateWindow: end"));
}

void ZTaskMan::showList()
{
	toLog(QString("showList"));
	
	lbProc->clear();
	
	buildProcList();
	
	#ifdef NEW_JAVA_LIST
    buildJavaListNew();
    #endif 
	
	lbProc->setCurrentItem(0);
	lbProc->update();
	
	toLog(QString("showList: end"));
}

void ZTaskMan::showInfo()
{
	toLog("showInfo");
	
	//set info
	setMemInfo();
	
	//update info on screen
	RAM->update();
	imgMEM->update();
	SWAP->update();
	imgSWAP->update();

	//update cpu info
	slotUpdate();
	lbProc->setFocus();
	
	// create timer for update CPU
	if ( timer == NULL )
	{
		timer = new QTimer( this );
    	connect( timer, SIGNAL(timeout()), this, SLOT(slotUpdate()) );
    }
    timer->start( 3000, false );

    toLog(QString("showInfo: end"));
}

void ZTaskMan::drawGraph(ZImage * img, int array[GRAPH_LEN], const QColor & color, bool fix)
{
	QPixmap * pix = new QPixmap(GRAPH_WIDTH,GRAPH_HEIGHT);
	pix->fill(Qt::black);
	QPainter * paint = new QPainter (pix);
	paint->setPen( color.dark() );
	int i;
	for ( i=1;i<GRAPH_WIDTH/GRAPH_LEN;i++ )
		paint->drawLine(i*GRAPH_LEN,0,i*GRAPH_LEN,GRAPH_HEIGHT);
	i = 0;
	while ( i < GRAPH_HEIGHT )
	{
		i += GRAPH_LEN;
		paint->drawLine(0,i,GRAPH_WIDTH,i);	
	}
	
	paint->setPen( color );
	uint n;
	n=graph_old;
	int old;
	old = array[DEC_EX_GRAPH(n)];
	
	if ( (fix) && (array[n]==0) )
		array[n] = old;
	paint->moveTo(0,GRAPH_HEIGHT-array[n]/(100/GRAPH_HEIGHT)); //image height 50 => divison by 2
	for ( i=1;i<GRAPH_LEN;i++ )
	{
		INC_GRAPH(n);
		if ( fix )
			if (array[n]==0)
				array[n] = old;
			else
				old = array[n];
		paint->lineTo((GRAPH_WIDTH/(GRAPH_LEN-1))*i,GRAPH_HEIGHT-((double)(GRAPH_HEIGHT-1)/100)*array[n]);
	}
	img->setPixmap(*pix);
	img->update();
}

void ZTaskMan::slotUpdate()
{
	#ifndef NO_SHOW_GRAPH
	if ( upGraph )
	{
		setMemInfo();
	} else
	{
		mem_graph[graph_old]=mem_graph[DEC_EX_GRAPH(graph_old)];
		swap_graph[graph_old]=swap_graph[DEC_EX_GRAPH(graph_old)];
	}
	#endif	
	
	int cur = getCPUn();
	CPUFreq->setText(QString::number(getCPUFreq())+"MHz\n"+QString::number(cur)+"%");
	CPUFreq->update();
	
	imgCPU->setFixedHeight( 40 - (int)(cur*0.40) );
	imgCPU->update();
	
	if ( settings->cfg_TimeInCaption )
		setTimeToCaption();
	
	#ifndef NO_SHOW_GRAPH
	if ( upGraph )
	{
		drawGraph(imgGraphProc, proc_graph, Qt::green, false);
		drawGraph(imgGraphMem,  mem_graph,  Qt::red);
		#ifndef CUTED_QT_AND_EZX
		drawGraph(imgGraphSwap, swap_graph, Qt::yellow);
		#else
		drawGraph(imgGraphSwap, swap_graph, Qt::blue); // On ZN5 no yellow const in libqte-mt(
		#endif
	}
	#endif	
}

void ZTaskMan::createProcMenu()
{
	QPixmap* pm  = new QPixmap();
	menu->clear();
	  	
	pm->load(settings->getProgramDir()+ "/img/show.png");
	menu->insertItem ( MENU_FIX+lng->getString("SHOW"), NULL, pm, true, 0, 0 );	//Show
	pm->load(settings->getProgramDir()+ "/img/quit.png");
	menu->insertItem ( MENU_FIX+lng->getString("QUIT"), NULL, pm, true, 1, 1 );	//Quit App
	menu->insertItem ( MENU_FIX+lng->getString("KILL"), NULL, pm, true, 2, 2 );	//Kill App	
	pm->load(settings->getProgramDir()+ "/img/priority.png");
	menu->insertItem ( MENU_FIX+lng->getString("PRIORITY") , NULL, pm, true, 3, 3 );	//Prioritet		
	pm->load(settings->getProgramDir()+ "/img/info.png");	
	menu->insertItem ( MENU_FIX+lng->getString("INFO"), NULL, pm, true, 4, 4 );	//Info
  	menu->insertSeparator(5, 5); 
  	pm->load(settings->getProgramDir()+ "/img/flag.png");	
 	menu->insertItem ( MENU_FIX+lng->getString("SETTING"), NULL, pm, true, 6, 6 );	//Settings
  	menu->insertSeparator(7, 7); 
  	pm->load(settings->getProgramDir()+ "/img/about.png"); 	
	menu->insertItem ( MENU_FIX+lng->getString("ABOUT"), NULL, pm, true, 8, 8 );	//About
	pm->load(settings->getProgramDir()+ "/img/exit.png");
	menu->insertItem ( MENU_FIX+lng->getString("EXIT"), NULL, pm, true, 9, 9 );	//Exit

	menu->connectItem ( 0, this, SLOT ( menu_procShow() ) );
	menu->connectItem ( 1, this, SLOT ( menu_procQuit() ) );
	menu->connectItem ( 2, this, SLOT ( menu_procKill() ) );
	menu->connectItem ( 3, this, SLOT ( menu_procPrioritet() ) );
	menu->connectItem ( 4, this, SLOT ( menu_procInfo() ) );
	menu->connectItem ( 6, this, SLOT ( menu_appSetting() ) );
	menu->connectItem ( 8, this, SLOT ( about() ) );	
	menu->connectItem ( 9, this, SLOT ( Quit() ) );
}

void ZTaskMan::createAppMenu()
{
	QPixmap* pm  = new QPixmap();
	menu->clear();
	  	
	pm->load(settings->getProgramDir()+ "/img/app_add.png");
	menu->insertItem ( MENU_FIX+lng->getString("ADD"), NULL, pm, true, 0, 0 );	//Add
	pm->load(settings->getProgramDir()+ "/img/app_del.png"); 
	menu->insertItem ( MENU_FIX+lng->getString("DEL"), NULL, pm, true, 1, 1 );	//Dell	
	pm->load(settings->getProgramDir()+ "/img/app_ren.png"); 
	menu->insertItem ( MENU_FIX+lng->getString("REN"), NULL, pm, true, 2, 2 );	//Ren			
  	menu->insertSeparator(3, 3); 
  	pm->load(settings->getProgramDir()+ "/img/flag.png");	
 	menu->insertItem ( MENU_FIX+lng->getString("SETTING"), NULL, pm, true, 4, 4 );	//Settings
  	menu->insertSeparator(5, 5); 
  	pm->load(settings->getProgramDir()+ "/img/about.png"); 	
	menu->insertItem ( MENU_FIX+lng->getString("ABOUT"), NULL, pm, true, 6, 6 );	//About
	pm->load(settings->getProgramDir()+ "/img/exit.png");
	menu->insertItem ( MENU_FIX+lng->getString("EXIT"), NULL, pm, true, 7, 7 );	//Exit

	menu->connectItem ( 0, this, SLOT ( menu_appAdd() ) );	
	menu->connectItem ( 1, this, SLOT ( menu_appDel() ) );
	menu->connectItem ( 2, this, SLOT ( menu_appRen() ) );	
	menu->connectItem ( 4, this, SLOT ( menu_appSetting() ) );
	menu->connectItem ( 6, this, SLOT ( about() ) );	
	menu->connectItem ( 7, this, SLOT ( Quit() ) );
}

#ifndef NO_SHOW_GRAPH
void ZTaskMan::createGraphMenu()
{
	QPixmap* pm  = new QPixmap();
	menu->clear();
	
  	pm->load(settings->getProgramDir()+ "/img/about.png"); 	
	menu->insertItem ( MENU_FIX+lng->getString("ABOUT"), NULL, pm, true, 0, 0 );	//About
	pm->load(settings->getProgramDir()+ "/img/exit.png");
	menu->insertItem ( MENU_FIX+lng->getString("EXIT"), NULL, pm, true, 1, 1 );	//Exit

	menu->connectItem ( 0, this, SLOT ( about() ) );	
	menu->connectItem ( 1, this, SLOT ( Quit() ) );
}
#endif

void ZTaskMan::slotPageChanged(QWidget*)
{
	int i = tabWidget->currentPageIndex();
	upGraph=false;
	
	switch (i)
	{
		case 0: createProcMenu(); break; 
		case 1:
		{
			createAppMenu();
			if ( lbApp->count() == 0 )
				buildAppList();
			break; 
		}  
		#ifndef NO_SHOW_GRAPH
		case 2:
		{
			createGraphMenu();
			if ( !upGraph )
			{
				upGraph = true;
				setMemInfo();
				slotUpdate();
			}
			tabWidget->setFocus();
			break; 
		}
		#endif
	}
}

void ZTaskMan::about()
{
	ZAboutDialog* aboutDlg = new ZAboutDialog();
	aboutDlg->exec();

	delete aboutDlg;
	aboutDlg = NULL;
}

void ZTaskMan::lbProcSel(int index)
{
	ZTaskItem* listitem = (ZTaskItem*)lbProc->item ( index ); 
	
	#ifdef NEW_JAVA_LIST
	if ( listitem->getType() == ITEM_JAVA )
		javaToTop( listitem->getUID() );
	else
	#endif
	#ifndef SWITCH_BY_UID
		pidToTop( listitem->getPID() );
	#else
		javaToTop( listitem->getUID() );
	#endif	

}

void ZTaskMan::lbAppSel(int index)
{
	if (index == 0)
	{
		goToIdle();	
		Quit();
	} else
	if (index > 1)
	{
		ZConfig cfg(appConf);
		
		ZTaskItem* listitem = (ZTaskItem*)lbApp->item ( index ); 
		int n = listitem->getReservedData();
		QString run = cfg.readEntry(QString("App"), QString("run"+QString::number(n)), "");
		if (run != "")
		{ 
			QRegExp rx( "{*-*-*-*-*}" ); //{????????-????-????-????-????????????}"
			if ( run.find(rx)!=-1 ) // if in "run" - AppId
			{
				toLog("Run app from AppId: "+run);
				#ifndef ES_EVENT
				QByteArray data;
				QDataStream stream( data, IO_WriteOnly );
				stream << (QUuid)run;
				QCopChannel::send( launcherChannel, QCString ( "launchApp4NoLockCheck" ), data );
				#else
				run = run.right(37).left(36); //AppId -> uid
				AM_LauncherClient::launchAppObject( run );
				#endif
			} else
			{
				toLog("Run app from path: "+run);
    			system( run );
			}
			cfg.flush();
			Quit();		
		} else
		{
			cfg.flush();
		}
	}
}

void ZTaskMan::slotRaise()
{
	toLog( "slot_Raise()");
	this->show();
}

void ZTaskMan::slotReturnToIdle( int reason )
{
	toLog( "slot_ReturnToIdle("+QString::number(reason)+")");
	this->hide();
}

void ZTaskMan::goToIdle()
{
	toLog("Miniumized all app");
	#ifdef RAISE_PHONE
	if (settings->cfg_SendGoToIDLE)
	#endif
	{
		//minimized java
		QByteArray data;
		QDataStream stream( data, IO_WriteOnly );
		
		stream << (int)RETURN_IDLE_BY_SLIDER;
		QCopChannel::send( SYSTEM_CHANNEL, QCString ( "ReturnToIdle(int)" ), data );
		
		#ifndef RAISE_PHONE
		hideAllJava();
		#endif		
	}
	
	#ifdef RAISE_PHONE
	if (settings->cfg_SendReaisePhone)
	{
		//hide no minimized nativ
		sendMes( pidPhone, "raise()" );	
	}
	#endif

	qApp->processEvents();
}

#ifndef RAISE_PHONE
void ZTaskMan::hideAllJava()
{
	if ( kvmStats )
	{
		toLog("!!! HIDE KVM !!!");	
		SETUP_Utility_Impl util;
		SETUP_Utility::SLIDE_CLOSED_SETTING_E iSliderAction = util.getSlideClosedSetting();

		if ( iSliderAction != SETUP_Utility::SLIDE_CLOSE_ALL )
			util.setSlideClosedSetting(SETUP_Utility::SLIDE_CLOSE_ALL);	
		
		QCopChannel::send( "EZX/System", "sliderClosed" );

		if ( iSliderAction != SETUP_Utility::SLIDE_CLOSE_ALL )
		{
			qApp->processEvents();
			qApp->processEvents();

			util.setSlideClosedSetting( iSliderAction );
		}
	}
}
#endif

#ifdef NEW_JAVA_LIST
void ZTaskMan::javaToTop( QString uid )
{
	toLog("javaToTop ()");
	if ( uid.isEmpty() )
		return;	

	toLog("Show java from uid ["+uid+"]");
	AM_LauncherClient::launchAppObject( uid );
	
	qApp->processEvents();
	Quit();  
}
#endif

void ZTaskMan::pidToTop(int pid)
{
	#ifndef RAISE_PHONE
	hideAllJava();
	#endif

	toLog("App ["+QString::number(pid)+"] show");
	sendMes( pid, "raise()" ); 

	qApp->processEvents();
	Quit();  
}

void ZTaskMan::sendMes(int pid, QString mes)
{
	#ifdef ES_EVENT
	ESCopChannel::send( QCString("EZX/Application/"+QString::number(pid)), QCString ( mes ), QByteArray(), false );
	#else
	QCopChannel::send( QCString("EZX/Application/"+QString::number(pid)), QCString ( mes ), QByteArray() );	
	#endif
}

void ZTaskMan::buildAppList()
{
	toLog("buildAppList");
	
	ZTaskItem* listitem = new ZTaskItem ( lbApp, QString ( "%I%M" ) );

    QPixmap pm;
    pm.load(settings->getProgramDir() + QString ( "/img/desktop.png" ));
    listitem->setPixmap ( 0, pm );
    listitem->appendSubItem ( 1, lng->getString("MINIALL"), true );//Minimized all	
    lbApp->insertItem ( listitem,0,true );	

	listitem = new ZTaskItem ( lbApp, QString ( "%I%M" ) );
    listitem->setSeparator("-",NULL);
	listitem->setSelectable(false);
    lbApp->insertItem ( listitem,1,true );
	
	toLog("buildAppList: Read app list of config...");
	
	ZConfig cfg(appConf);
	
	QString name;
	QString ico;
	for (int i=0;i<20;i++)
	{
		name = cfg.readEntry(QString("App"), QString("name"+QString::number(i)), "");
		if (name != "")
		{
			QPixmap pm;
			listitem = new ZTaskItem ( lbApp, QString ( "%I%M" ) );

			ico = cfg.readEntry(QString("App"), QString("icon"+QString::number(i)), "");
	
			#ifndef NEW_PLATFORM
			if ( ico.endsWith(".g") )
			#else
			if ( ico.endsWith(".g") || ico.endsWith(".k") )
			#endif
			{
				RES_ICON_Reader res;
				pm = res.getIcon(ico, false);
			} else
				pm.load( ico );

			if ( pm.isNull() ) 
				pm.load( settings->getProgramDir() + QString ( "/img/run_app.png" ) );
			else
				imgSetMaxSize(&pm, 19);
			listitem->setPixmap ( 0, pm );
			listitem->appendSubItem ( 1, name , true );	
			listitem->setReservedData ( i );
			lbApp->insertItem ( listitem,i+2,true );
		}
	}
	
	cfg.flush(); 
	toLog("buildAppList: end");   	    
}

#ifdef NEW_JAVA_LIST
void ZTaskMan::buildJavaListNew()
{
	toLog("buildJavaListNew");  
	kvmStats=0;
	AM_RESULT_CODE_T result = AM_LauncherClient::getAppStates(AM_AppLnk::JavaApp, runJavaList);
	if ( result == AM_RESULT_SUCCESS )
	{
		toLog("@ Java list geted! Count item: "+QString::number(runJavaList.count()));
		
		ZConfig confCARD ( CARD_REGISTRY, false );
		ZConfig conINSTALL ( INSTALLED_DB, false );
		
		QString name;
		QString icon;
		QPixmap pm;	

		toLog("Start add java");
		AM_AppLaunchedList::Iterator it;
		for ( it = runJavaList.begin(); it != runJavaList.end(); ++it )
		{
			kvmStats=kvmStats||((*it).state&RUNNING_ON_FOREGROUND);
			
			name = confCARD.readEntry ( (*it).uid, "Name", "" );
			if (name != "")
			{
				icon = confCARD.readEntry ( (*it).uid, "Icon", "" );
				if (icon=="")
					icon = confCARD.readEntry ( (*it).uid, "BigIcon", "" );
				if ( icon != "" && icon.find(".g") == -1 && icon.find(".k") == -1&& !QFile::exists(icon) )
				{
					icon = confCARD.readEntry ( (*it).uid, "Directory", "" ) + "/" + icon;
				}					
			} else
			{
				name = conINSTALL.readEntry ( (*it).uid , "Name", "" );
				if (name == "")
					continue;	
				icon = conINSTALL.readEntry ( (*it).uid, "Icon", "" );
				if (icon=="")
					icon = conINSTALL.readEntry ( (*it).uid, "BigIcon", "" );
					
				if ( icon != "" && icon.find(".g") == -1 && icon.find(".k") == -1 && !QFile::exists(icon) )
				{
					icon = conINSTALL.readEntry ( (*it).uid, "Directory", "" ) + "/" + icon;
				}													
			}
			
			if (icon=="")
				icon = settings->getProgramDir() + QString ( "/img/java.png" );

			ZTaskItem* listitem = new ZTaskItem ( lbProc, QString ( "%I%M" ) );						
			
			#ifndef NEW_PLATFORM
			if ( icon.find(".g") != - 1)
			#else
			if ( (icon.find(".g") != -1) || (icon.find(".k") != -1) )
			#endif
			{
				RES_ICON_Reader res;
				pm = res.getIcon(icon, false);
			} else
				pm.load( icon );

			if ( pm.isNull() ) 
				pm.load( settings->getProgramDir() + QString ( "/img/java.png" ) );
			else
				imgSetMaxSize(&pm, 19);	
	
			listitem->setPixmap ( 0, pm );
			listitem->appendSubItem ( 1, QString( name ), true );
			listitem->setJava(true);
			listitem->setUID((*it).uid);		
			lbProc->insertItem ( listitem, 0, true );	
		}
	}
	toLog("buildJavaListNew: end");  
}
#endif

void ZTaskMan::pidQuit(int pid)
{ 
	sendMes( pid, "quickQuit()" );      
}

bool ZTaskMan::pidKill( int pid)
{
    if ( ::kill( pid, SIGKILL ) < 0 )
    {
		toLog( "Error when kill a pid ["+QString::number(pid)+"]" );
		return false;
    }
    toLog( "Killed a pid ["+QString::number(pid)+"]" );
    return true;
}

bool ZTaskMan::processIsRuning(pid_t pid)
{
    FILE * f;
    if ( (f = fopen(QCString("/proc/"+QString::number(pid)+"/cmdline"), "r")) == NULL )
		return false;
	else
	{
		fclose(f);
		return true;
	}
}

void ZTaskMan::menu_appAdd()
{
	ZAddAppDlg* addDlg = new ZAddAppDlg( );
	addDlg->exec();

	if ( addDlg->result() == 1 )
	{
		lbApp->clear();
		buildAppList(); 
	}

	delete addDlg;
	addDlg = NULL;
}

void ZTaskMan::menu_appDel()
{
	int n = lbApp->currentItem();
	if (n-2 < 0) 
	{
		return;
	}
	
	ZTaskItem* listitem = (ZTaskItem*)lbApp->item ( n ); 
	
	n = listitem->getReservedData();
	
	toLog("Del index = "+QString::number(n));
	ZConfig cfg(appConf);
	
	cfg.writeEntry("App", "name"+QString::number(n), "");
	cfg.writeEntry("App", "run"+QString::number(n), "" );
	cfg.writeEntry("App", "icon"+QString::number(n), "" );
	
	cfg.flush();
	
	lbApp->clear();
	buildAppList();	
}

void ZTaskMan::menu_appRen()
{
	int n = lbApp->currentItem();
	if (n-2 < 0) 
	{
		return;
	}
	
	ZTaskItem* listitem = (ZTaskItem*)lbApp->item ( n ); 

	n = listitem->getReservedData();
	
	toLog("Ren index = "+QString::number(n));
	ZConfig cfg(appConf);
	
	ZSingleCaptureDlg * dlg = new ZSingleCaptureDlg("", lng->getString("DLG_REN"), ZSingleCaptureDlg::TypeLineEdit, this);
	
	ZLineEdit *line = dlg->getLineEdit();	
	line->setText( cfg.readEntry(QString("App"), QString("name"+QString::number(n)), "") );
	if ( dlg->exec() == QDialog::Accepted )
	{
		QString text = line->text();
		cfg.writeEntry("App", "name"+QString::number(n), text);
		
		cfg.flush();
		
		lbApp->clear();
		buildAppList();	
		
		delete line;
		delete dlg;
		
		return;		
	}
	delete line;
	delete dlg;
	
	cfg.flush();
}

void ZTaskMan::menu_procShow()
{
	int i = lbProc->currentItem();
	if (i == -1) 
		return;

	lbProcSel(i);
}

void ZTaskMan::menu_procQuit()
{
	int i = lbProc->currentItem();
	if (i == -1) 
	{
		return;
	}

	ZTaskItem* listitem = (ZTaskItem*)lbProc->item ( i ); 

	#ifdef NEW_JAVA_LIST
	if ( listitem->getType() == ITEM_JAVA )
		return;
	#endif

	int pid = (int)listitem->getPID();

	pidQuit( pid );
	
    for ( int i = 0; i < 50; i++ )
    {
        if ( !processIsRuning( pid ) )
        {
			lbProc->removeItem ( lbProc->currentItem() );
			lbProc->setCurrentItem(0);
            break;
        }
		
		for ( int j = 0; j < 10; j++ )
		{
			qApp->processEvents();
			usleep(50);
		}
    }
}

void ZTaskMan::menu_procKill()
{	
	int i = lbProc->currentItem();
	if (i == -1) 
	{
		return;
	}

	ZTaskItem* listitem = (ZTaskItem*)lbProc->item ( i ); 

	#ifdef NEW_JAVA_LIST
	if ( listitem->getType() == ITEM_JAVA )
		return;
	#endif
	
	int pid = (int)listitem->getPID();

	pidKill( pid );

	lbProc->removeItem ( lbProc->currentItem() );
	lbProc->setCurrentItem(0);
}

void ZTaskMan::menu_procInfo()
{
	int i = lbProc->currentItem();
	if (i == -1) 
	{
		return;
	}

	ZTaskItem* listitem = (ZTaskItem*)lbProc->item ( i ); 

	int pid = (int)listitem->getPID();

	#ifdef NEW_JAVA_LIST
	if ( listitem->getType() == ITEM_JAVA )
		return;
	#endif

	ZProcInfoDlg* infoDlg = new ZProcInfoDlg( pid );
	infoDlg->exec();

	delete infoDlg;
	infoDlg = NULL;
}

void ZTaskMan::menu_procPrioritet()
{
	int i = lbProc->currentItem();
	if (i < 0) 
		return;

	ZTaskItem* listitem = (ZTaskItem*)lbProc->item ( i ); 

	int pid = (int)listitem->getPID();

	#ifdef NEW_JAVA_LIST
	if ( listitem->getType() == ITEM_JAVA )
		return;
	#endif
	
	int pr = getpriority(PRIO_PROCESS, pid);

	ZNumSelect* dlg = new ZNumSelect( this, lng->getString("PRIORITY")+":" );
	
	dlg->setNum(pr);

	if ( dlg->exec() == 1 )
		setpriority(PRIO_PROCESS, pid, dlg->getNum());

	delete dlg;
	dlg = NULL;
}

void ZTaskMan::menu_appSetting()
{
	toLog("menu_appSetting: Start");
	
	ZAppSetting* settingDlg = new ZAppSetting( );
	settingDlg->exec();

	if ( settingDlg->result() == 1 )
	{
		lbProc->clear();
		
		QFont font ( qApp->font() );
		font.setPointSize ( settings->cfg_ListFontSize );
		lbProc->setItemFont (ZListBox::LISTITEM_REGION_C, font, true );
		lbProc->setItemFont (ZListBox::LISTITEM_REGION_C, font, false );
		lbProc->setItemFont (ZListBox::LISTITEM_REGION_A, font, true );
		lbProc->setItemFont (ZListBox::LISTITEM_REGION_A, font, false );
		lbProc->setItemFont (ZListBox::LISTITEM_REGION_B, font, true );
		lbProc->setItemFont (ZListBox::LISTITEM_REGION_B, font, false );			
		lbApp->setItemFont (ZListBox::LISTITEM_REGION_C, font, true );
		lbApp->setItemFont (ZListBox::LISTITEM_REGION_C, font, false );	
		lbApp->setItemFont (ZListBox::LISTITEM_REGION_B, font, true );
		lbApp->setItemFont (ZListBox::LISTITEM_REGION_B, font, false );	
		lbApp->setItemFont (ZListBox::LISTITEM_REGION_A, font, true );
		lbApp->setItemFont (ZListBox::LISTITEM_REGION_A, font, false );	
		
		font.setPointSize ( settings->cfg_PanelFontSize );
		CPUFreq->setFont( font );
		RAM->setFont( font );
		SWAP->setFont( font );

		if ( settings->cfg_UserFont )
		{	
			CPUFreq->setFontColor(settings->cfg_FontColor);
			RAM->setFontColor(settings->cfg_FontColor);
			SWAP->setFontColor(settings->cfg_FontColor);
		}
		
		buildProcList();
		
		lbApp->clear();
		buildAppList();
		
		lbProc->setFocus();
	}

	delete settingDlg;
	settingDlg = NULL;
	
	toLog("menu_appSetting: End");
}

void ZTaskMan::addProc(uint pid, QString nameProc, bool cmd )
{
	int i;
	for (i=0;i<(int)lbProc->count();i++)
	{
		ZTaskItem* listitem = (ZTaskItem*)lbProc->item ( i ); 
		if ( listitem->getPID() == pid )
			return;
	}
	
	ZTaskItem* listitem = new ZTaskItem ( lbProc, QString ( "%I%M" ) );
	QPixmap pm;
	if ( cmd )
	{
		pm.load(settings->getProgramDir() + QString ( "/img/sh.png" ));
		listitem->appendSubItem ( 1, QString( extractExecName( nameProc ) ), true );
	} else
	#ifdef FLOWPLAYER
	if ( (jname.find("flowplayer") > -1) )
	{	
		pm.load(settings->getProgramDir() + QString ( "/img/menu.png" ));
		listitem->setType( ITEM_SPECIAL );
		listitem->appendSubItem ( 1, QString( lng->getString("FLOWPLAYER") ), true );				
	} else
	#endif
	if ( nameProc.find("kvm") > -1 )
	{
		#ifndef NEW_JAVA_LIST
		listitem->setJava(true);
		
		QString picName;
		
		int n = nameProc.find("-launch");
		if ( n > -1 )
		{
			//Get java id from run line
			nameProc.remove(0,n+7);
			while ( nameProc[0]==' ' )
				nameProc.remove(0,1);
			n = nameProc.find(" ");
			if ( n > -1 )
				nameProc.remove(n,nameProc.length());		
			//Fix java id, if need		
			while ( nameProc.length() < 5 )
				nameProc = "0"+nameProc;
			//Set pixmap and name
			picName = "/mmc/mmca1/.system/"+ settings->JavaFolderInSD +"/DownloadApps/MIDlet"+nameProc+"/Files/MIDlet-1.png" ;
			if ( !QFile::exists(picName) )
			{
				picName = "/ezxlocal/download/mystuff/.system/java/DownloadApps/MIDlet"+nameProc+"/Files/MIDlet-1.png";
				if ( !QFile::exists(picName) )
					picName = "/ezxlocal/download/mystuff/.system/java/SystemApps/MIDlet"+nameProc+"/Files/MIDlet-1.png";
			}				
			toLog("LoadIMG: "+picName);
			picName = settings->mapJava[nameProc];
			if ( picName.length() == 0 )
				picName = "Java ID:'"+nameProc+"'";
			listitem->appendSubItem( 1, QString::fromUtf8(picName));					
		}
		
		if ( pm.isNull() )
			pm.load( settings->getProgramDir() + QString ( "/img/java.png" ) );
		else
			imgSetMaxSize(&pm, 19);	
		#else
		listitem->appendSubItem ( 1, QString( "Java Machine" ), true );
		pm.load(settings->getProgramDir() + QString ( "/img/java.png" ));
		listitem->setType(ITEM_SPECIAL);
		#endif
	} else
	{	
		QString nname, ico;
		for ( mapIconNativ::Iterator it = settings->mapIcon.begin( ); it != settings->mapIcon.end( ); it++) 
		{ 
			nname=it.key();
			if ( (!it.data().t() && ((nameProc.length()<3)?(nname == nameProc):(nname.find(nameProc)!=-1))) || (it.data().icon() && nname == nameProc) )
			{
				ico = it.data().icon();
				nname = it.data().name();
				#ifndef NEW_PLATFORM
				if ( ico.endsWith(".g") )
				#else
				if ( ico.endsWith(".g") || ico.endsWith(".k") )
				#endif
				{
					RES_ICON_Reader res;
					pm = res.getIcon(ico, false);
				} else
					pm.load( ico );
				
				if ( !nname.isEmpty() )
					nameProc = nname;
	
				#ifdef SWITCH_BY_UID
				listitem->setUID( it.data().uid() );
				#endif
				break;						
			}		
		}
		
		if ( pm.isNull() ) 
			pm.load( settings->getProgramDir() + QString ( "/img/app.png" ) );
		else
			imgSetMaxSize(&pm, 19);
		listitem->appendSubItem ( 1, QString( nameProc ), true );
	}
	listitem->setPixmap ( 0, pm );
	listitem->setPID ( (uint)pid );						
	#ifdef NEW_PLATFORM				
	lbProc->insertItem ( listitem, 1, true );
	#else
	lbProc->insertItem ( listitem, 0, true );
	#endif
}

QString ZTaskMan::extractExecName(QString nameProc)
{
	while ( nameProc.find("/lib/")>=0 )
		nameProc.remove(1, nameProc.find(" ") );
	
	nameProc.remove(nameProc.find(" "), nameProc.length() );
	
	if (nameProc.find(" ")<0 || nameProc.find(" ") > nameProc.find("/"))
	{
		while ( nameProc.find("/") > -1 )
		{
			nameProc.remove(0,nameProc.find("/")+1 );
		}	
	}		

	return nameProc;
}

bool ZTaskMan::procFilter(QString nameProc)
{
	if ( settings->cfg_FiltrProc == 0 )
	{
		if ( nameProc.find("zTaskMan") >= 0 )
		{
			return 0;
		}
		return 1;		
	}

	if ( settings->cfg_HideProcList.find( "!" + nameProc + "!" ) >= 0 )
	{
		return 0;	
	} 
	
	return 1;
}

bool ZTaskMan::procFilterSh(QString nameProc)
{
	if ( nameProc.find("/.Programs/") >= 0 || 
			nameProc.find("/download/mystuff/") >= 0 || 
				nameProc.find("/mmc/mmca") >= 0 )
	{
		return 1;
	}
		
	return 0;
}

void ZTaskMan::buildProcList()
{
	//************************* Get runing proc ************************
	toLog("Get runing proc");
	lbProc->clear();
	#ifdef RAISE_PHONE
	pidPhone = 0;
	#endif

	DIR *dir;
	struct dirent *entry;
	char *name;
	int n;
	char buf[255];
	FILE *fp;
	int pid;
	char *cmd;
	
	QString jname;	

	dir = opendir("/proc");
	if(!dir)
		toLog("Can't open /proc");
		
	for(;;) 
	{
		cmd = 0;
		
		if((entry = readdir(dir)) == NULL) 
		{
			closedir(dir);
			dir = 0;
			toLog("Get runing proc: End");
			break;
		}
		name = entry->d_name;
		if (!(*name >= '0' && *name <= '9'))
			continue;

		pid = atoi(name);
		if ( pid<100 )
			continue;
			
		#ifdef ES_EVENT
		if ( ESCopChannel::isRegistered( QCString(APP_CHANNEL_PREF + QString::number(pid)) ) )		
		#else
		if ( QCopChannel::isRegistered(QCString(APP_CHANNEL_PREF + QString::number(pid))) )
		#endif
		{
			if((fp = fopen(QCString("/proc/"+QString::number(pid)+"/cmdline"), "r")) == NULL)
				continue;
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
			
			// add proces in list
			if (cmd != 0 && cmd[0] != 0 )
			{
				jname = QString(cmd).stripWhiteSpace();
			
				#ifdef RAISE_PHONE
				//Save pid of phone proces for minimized all app
				if ( pidPhone == 0 && jname.find("SYSqtapp/phone/phone") > -1 )
				{
					pidPhone = pid;
					toLog("pidPhone="+QString::number(pidPhone));			
				}
				#endif			
				
				jname = extractExecName(jname);
				
				if ( procFilter(jname) )			
					addProc( pid, jname );
			}
		} else
		if ( settings->cfg_ShowAppNoWindow )
		{
			if((fp = fopen(QCString("/proc/"+QString::number(pid)+"/cmdline"), "r")) == NULL)
				continue;
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
			if ( procFilterSh(QString(cmd)) )
				addProc(pid, QString(cmd), true);
		}
	}			
}

bool ZTaskMan::eventFilter(QObject* o, QEvent* pEvent)
{ 
    if (QEvent::KeyPress == pEvent->type())
    {
       QKeyEvent *pKey = (QKeyEvent*)pEvent;

       if (pKey->key() == KEY_RED)
       {
			Quit();
			return true;
	   } 
	} else
    if (QEvent::KeyRelease == pEvent->type())
    {
       QKeyEvent *pKey = (QKeyEvent*)pEvent;
	 
	   if (o == lbProc)
	   {
			if ( settings->cfg_GreenShowInfo && pKey->key() == KEY_GREEN )
				menu_procInfo();
			if ( pKey->key() == KEY_C )
			{
				switch (settings->cfg_CAction)
				{
					case 1:
						menu_procKill();
						break;
					case 2:
						menu_procQuit();
						break;
					default:
						break;
				}
			}
	   }
	} 
	#ifndef NO_SHOW_GRAPH
	else
	if ( ( QEvent::FocusIn == pEvent->type() ) && ( o == gp ))
		tabWidget->setFocus();
	#endif
    return QWidget::eventFilter( o, pEvent ); 
}

int ZTaskMan::getCPUFreq()
{ 
	unsigned long long freq = 0; 	
	FILE *f = fopen("/sys/mpm/op", "r");
	if (fscanf(f, "%llu", &freq) < 1) 
	{ 
		fclose(f);
		return -1;
	} 
	fclose(f);
	return (int) freq;
}

int ZTaskMan::getCPUn()
{ 
	unsigned long long cpu = 0; 
	unsigned long long nice = 0; 
	unsigned long long system = 0; 
	unsigned long long idle = 0; 

	FILE *f = fopen("/proc/stat", "r");
	if (fscanf(f, "cpu %llu %llu %llu %llu", &cpu, &nice, &system, &idle) < 4) 
	{ 
		fclose(f);
		return -1;
	} 
	fclose(f);

	unsigned long long busy = cpu + nice + system;
	unsigned long long work = busy + idle;

	double usage = 100.0 * (busy-oldBusy) / (double) (work-oldWork);

	oldBusy = busy;
	oldWork=work;

	if (usage > 100)
		usage = 100;

	#ifndef NO_SHOW_GRAPH
	proc_graph[graph_old]=(int)usage;
	INC_GRAPH(graph_old);
	#endif

	return (int)usage;
}

void ZTaskMan::setMemInfo()
{
	toLog("setMemInfo");
	int MemFree=0;
	int MemTotal=0;
	int SWAPFree=0;
	int SWAPTotal=0;	
	QStringList lines;
	toLog("get data");
	FILE * f;
	f=fopen( "/proc/meminfo", "rt" );
	char s[20];
	int d;
	char s2[5];
	fscanf(f, "%s\t%d %s",s,&d,s2);
	while ( !feof(f) )
	{
		if ( strcmp(s,"MemFree:") == 0  )
			MemFree = d;
		else
		if ( strcmp(s,"Cached:") == 0  )
			MemFree += d;
		else
		if ( strcmp(s,"MemTotal:") == 0  )
			MemTotal = d;
		else
		if ( strcmp(s,"SwapFree:") == 0  )
			SWAPFree = d;
		else
		if ( strcmp(s,"SwapTotal:") == 0  )
			SWAPTotal = d;		
		else
		if ( strcmp(s,"SwapCached:") == 0  )
			SWAPFree += d;				
		fscanf(f, "%s\t%d %s",s,&d,s2);
	}
	fclose(f);
    toLog("set data");   
    #ifndef NO_SHOW_GRAPH
	mem_graph[graph_old]= (int)((double)(MemFree*100)/MemTotal);
	swap_graph[graph_old]=(int)((double)(SWAPFree*100)/SWAPTotal);
	imgMEM->setFixedWidth( 48-mem_graph[graph_old]/(100/48) );
	imgSWAP->setFixedWidth( 48-swap_graph[graph_old]/(100/48) );
	#else
	imgMEM->setFixedWidth( (int) ((48/((double)MemTotal))*(MemTotal-MemFree)) );
	imgSWAP->setFixedWidth( (int)((48/((double)SWAPTotal))*SWAPFree) );
	#endif
	RAM->setText(QString::number( ((double)MemFree)/1024,'f',1)+"Mb");
	SWAP->setText(QString::number(((double)SWAPFree)/1024,'f',1)+"Mb");
	toLog("setMemInfo end");	
}

void ZTaskMan::Quit()
{
	if ( isDaemon )
	{
		hide();
		//Stop update info (free mem, cpu freq and etc)
		delete timer;
		timer = NULL;
		#ifndef NO_SHOW_GRAPH
		//Free mem on graphics
		imgGraphProc->setPixmap(NULL);
		imgGraphMem->setPixmap(NULL);
		imgGraphSwap->setPixmap(NULL);
		memset(proc_graph, 0, GRAPH_LEN*sizeof(int));
		memset(swap_graph, 0, GRAPH_LEN*sizeof(int));
		memset(mem_graph, 0, GRAPH_LEN*sizeof(int));
		graph_old=0;
		#endif
		//Free mem on app list
		lbProc->clear();
	} else
		qApp->quit();
}

void ZTaskMan::Show()
{
	if ( isHidden() )
	{
		show();
		tabWidget->setCurrentPage(0);
		showList();
		showInfo();
	} else
		show();		
}

void ZTaskMan::setTimeToCaption()
{
	time_t currtime;
	time(&currtime);
	
	int h = localtime(&currtime)->tm_hour;
	int m = localtime(&currtime)->tm_min;
	
	setMainWidgetTitle ( QString("zTaskMan [%1:%3%2]").arg(h).arg(m).arg(m<10?"0":"") );	
}

