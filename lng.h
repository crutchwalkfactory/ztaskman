//
// C++ Implementation: ZTaskMan - lng file
//
// Description: task manager
//
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2009
//

#include <qobject.h>
#include <unistd.h>
#include <stdlib.h>

#include <ZComboBox.h>
#include <ZConfig.h>

class ZLng
{
	public:
		ZLng();
		~ZLng();

		QString getString(QString str);
		
		QString ProgDir;

		void loadLngFile(QString fname );
		void getLngList(ZComboBox *list, QString cur);
		QString getLngFileName(QString lname);
		void autoLoadLng();
		void loadLngFullNameFile(QString fname);
		void setCurrentLng(QString lng);

		QString getCurrentLng();

	private:
		QString CurLng;
		ZConfig * cLng;
};

//ZAboutDlg
#define LNG_VER "0.4.2"
//#define LNG_THANKS "Thanks to Abram, BeZ"
#ifdef EZX_VE66
#define LNG_THANKS   "Special build for PearlMod!"
#elif EZX_ZN5
#define LNG_THANKS   "Special build for ZE!"
#else
#define LNG_THANKS "Thanks to Abram, BeZ"
#endif
//#define LNG_THANKS "Special build for Vacuum 1.6!"
