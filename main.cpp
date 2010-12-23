//
// C++ Implementation: TestMain
//
// Description:
//
//
// Author: BeZ <bezols@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "zTaskMan.h"
#include "lng.h"
#include "ZDefs.h"
#include "ZLoadSettings.h"
#include <ZApplication.h>
#include <qcopchannel_qws.h>

#ifdef ES_EVENT
#include <ES_API.h>
#include <ES_PublisherAPI.h>
#endif

ZTaskMan *zTaskMan;
ZApplication* app;
ZLng* lng;
ZSettings * settings;
bool isDaemon;
bool onQuit;

int main ( int argc, char **argv )
{
	app = new ZApplication ( argc, argv );
	
	int ret = -1;
	
	isDaemon = false;
	
	zTaskMan = NULL;
  
	if ( QString(argv[1]) == QString("-daemon")  ) 
		isDaemon = true;
	
	if ( QCopChannel::isRegistered(ZTASKMAN_START) )
	{
		QCopChannel::send( ZTASKMAN_START, "show()" );
		ret = 0;
	}
	
	if ( ret == -1 )
	{
		lng = new ZLng();
		settings = new ZSettings();		
			
		//load description of java and nativ
		settings->loadAppDB();
		
		if ( isDaemon )
		{
			qDebug("isDaemon");
			settings->startDaemonChenel();
			onQuit = false;
			while ( !onQuit )
			{		
				ret = app->exec();
				delete zTaskMan;
				zTaskMan = NULL;
			}
		} else
		{
			zTaskMan = new ZTaskMan ( NULL, NULL );
			ret = app->exec();
		}
		
		delete lng;
		lng = NULL;   
		delete settings;
		settings = NULL;
	}

	delete app;
	app = NULL;
	return ret;
}
