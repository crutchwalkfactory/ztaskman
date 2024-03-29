//
// Project: zTaskMan
//
// C++ Implementation:
//
// Description: main function
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
			ret = app->exec();
			if ( zTaskMan )
			{
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
