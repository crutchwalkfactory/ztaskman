#include "lng.h"

#include "ZDefs.h"
#include "ZUtils.h"

#include <dirent.h>

#include <qfile.h>

ZLng::ZLng()
{
	CurLng = "";
	cLng = NULL;
}

ZLng::~ZLng()
{
	if ( cLng )
		delete cLng;
}

QString ZLng::getString(QString str)
{
	if ( cLng )
		return QString::fromUtf8(cLng->readEntry("Strings", str, str).utf8());
	return str;
}

QString ZLng::getCurrentLng()
{
	return CurLng;
}

void ZLng::setCurrentLng(QString lng)
{
	CurLng=lng;
}

void ZLng::loadLngFile(QString fname)
{
	toLog("Lng: "+fname);
	cLng = new ZConfig( fname );
}

int ZLng::getLngList(QStringList *list)
{
	toLog("getLngList");
	static DIR *dir;
	struct dirent *entry;
	char *name;
	QString qname;
	QString lang;
	
	int NumCurLng=0;		

	dir = opendir( ProgDir + "/lng");
	if(!dir)
	{
		toLog("getLngList: Can't open SD Card");
	} else	
	{
		int i;
		i=0;
		for(;;) 
		{	
			if((entry = readdir(dir)) == NULL) 
			{
				closedir(dir);
				dir = 0;
				break;
			}	
			name = entry->d_name;
			qname = QString(name);
			if (qname.find(".lng")>-1)
			{
				toLog("getLngList: LNG->"+ qname);
				ZConfig cfg( QString( ProgDir + "/lng/"+ qname) );
				lang = cfg.readEntry(QString("Info"), QString("LNG"), "-").utf8();
				cfg.flush();
				list->append(QString::fromUtf8(lang));
				
				if (qname==CurLng)
					NumCurLng = i;
				
				i++;
			}
		}	
	}
	toLog("getLngList: end");
	
	return NumCurLng;
}

QString ZLng::getLngFileName(QString lname)
{
	toLog("getLngFileNam");
	static DIR *dir;
	struct dirent *entry;
	char *name;
	QString qname;
	QString lang;		

	dir = opendir( ProgDir + "/lng");
	if(!dir)
	{
		toLog("getLngList: Can't open SD Card");
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
			name = entry->d_name;
			qname = QString(name);
			if (qname.find(".lng")>-1)
			{
				toLog("getLngFileNam: LNG->"+ qname);
				ZConfig cfg( QString( ProgDir + "/lng/"+ qname) );
				lang = cfg.readEntry(QString("Info"), QString("LNG"), "-");
				if (lang == lname)
				{
					cfg.flush();
					toLog("getLngFileNam: "+qname);
					return qname;
				}
				cfg.flush();
			}
		}	
	}
	toLog("getLngFileNam: end");
	return "";
}

void ZLng::autoLoadLng()
{
	ZConfig cfg( "/ezxlocal/download/appwrite/setup/ezx_system.cfg" );
	QString curLng = cfg.readEntry(QString("SYS_SYSTEM_SET"), QString("LanguageType"), "");
	if ( curLng.length() > 1 )
		curLng = QString(curLng[0]) + QString(curLng[1])+".lng";
	qDebug("autoLoadLng: current system lng -"+curLng);
	if ( QFile::exists(LNG_PATH + curLng) )
	{
		loadLngFile(LNG_PATH + curLng);
	} else
	{
		qDebug("autoLoadLng: Not found. Load default lng.");
		loadLngFile(LNG_PATH + "en.lng");		
	}
}
