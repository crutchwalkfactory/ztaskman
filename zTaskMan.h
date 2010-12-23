//
// C++ Interface: zTaskMan
//
// Description:
//
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2008-2010
//

#ifndef ZGUI_H
#define ZGUI_H

#include <qobject.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <ZApplication.h>
#include <ZMessageDlg.h>
#include <ZListBox.h>
#include <ZKbMainWidget.h>
#include <ZNavTabWidget.h>
#include <ZScrollPanel.h>
#include <qmap.h>
#include <ZOptionsMenu.h>
#ifdef OLD_PLATFORM
#include <ZScrollView.h>
#else
#include <qscrollview.h>
#endif
#include <ZFormContainer.h>
#include <ZImage.h>

#include <qcopchannel_qws.h>
#include <qthread.h>
#include <qtimer.h>

#ifdef ES_EVENT
#include <ESCopChannel.h>
#include <ES_SubscriberAPI.h>
#endif

#ifdef NEW_JAVA_LIST
#include <qvaluelist.h>

#include <AM_AppLnk.h>
#include <AM_LauncherClient.h>

typedef QValueList<AM_LauncherClient::AM_ApplicationState> AM_AppLaunchedList;
#endif

#include "ZDefs.h"

class ZTaskMan : public ZKbMainWidget
{
    Q_OBJECT
  public:
    ZTaskMan (QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ZTaskMan();
    
	ZListBox *lbProc;
    ZListBox *lbApp;
    #ifdef RAISE_PHONE
	int pidPhone;
	#endif

    void CreateWindow(QWidget* parent);
    void toLog(QString mes, bool r1 = false , bool r2 = false );
    
    QString extractExecName(QString nameProc);
	bool procFilter(QString nameProc);    
	bool procFilterSh(QString nameProc);
	
	void buildAppList();
    void buildProcList();
    #ifdef NEW_JAVA_LIST
    void buildJavaListNew();
    #endif
    void addProc(uint pid, QString nameProc, bool cmd = false );
    
	void sendMes(int pid, QString mes);      
	bool processIsRuning( pid_t pid );
	bool pidKill( int pid);
	void pidQuit( int pid );
	#ifdef NEW_JAVA_LIST
	void javaToTop(QString uid);
	#endif
	void pidToTop(int pid);	
	void goToIdle();
	
	void createProcMenu();
	void createAppMenu();
	#ifndef NO_SHOW_GRAPH
	void createGraphMenu();
	#endif
	    
	int getCPUFreq();
	int getCPUn();
	void setMemInfo();
	
	void Show();
	void showInfo();
	void showList();

	void setTimeToCaption();
	
  public slots:
    void about();
    void lbProcSel(int index);
    void lbAppSel(int index);    
          
    void slotRaise();
    void slotReturnToIdle( int reason );
    
    void menu_procShow();
    void menu_procQuit();  
    void menu_procKill(); 
    void menu_procInfo();   
    void menu_procPrioritet();    
    void menu_appSetting();
    void menu_appDel();
    void menu_appAdd();
    void menu_appRen();	
    
    void slotPageChanged(QWidget* AWidget);
        
    void Quit();
    
    void slotUpdate();

  private:
  	ZNavTabWidget *tabWidget;
    ZSoftKey *softKey;
    ZOptionsMenu* menu;
	ZLabel *CPUFreq;
	ZLabel *CPUn;
	ZLabel *RAM;
	ZScrollPanel * sp;	
	#ifdef OLD_PLATFORM
	ZScrollView * sv;
	#else
	QScrollView * sv;
	#endif	
	ZImage * imgMEM;
	ZImage *  imgCPU; 	
	ZLabel *SWAP;
	ZImage *  imgSWAP;
	int oldBusy, oldWork;

	#ifdef NEW_JAVA_LIST
	AM_AppLaunchedList runJavaList;
	#endif
	
	QTimer *timer;
	bool isShow;
	
	#ifndef NO_SHOW_GRAPH
	int proc_graph[GRAPH_LEN];
	int swap_graph [GRAPH_LEN];
	int mem_graph [GRAPH_LEN];
	uint graph_old;
	ZImage * imgGraphProc;
	ZImage * imgGraphMem;	
	ZImage * imgGraphSwap;	
	ZScrollPanel * gp;
	bool upGraph;
	
	void drawGraph(ZImage * img, int array[GRAPH_LEN], const QColor & color, bool fix=true);
	#endif
    
  protected:
    bool  eventFilter( QObject *, QEvent * );
};

#endif
