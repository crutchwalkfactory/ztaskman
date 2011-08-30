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

#ifndef ZSETTINGDLG_H
#define ZSETTINGDLG_H

#include "BaseDlg.h"
#include <ZListBox.h>

class ZAppSetting : public MyBaseDlg 
{
	Q_OBJECT

public:
	ZAppSetting();
	~ZAppSetting();

	ZListBox * zlbSettings;

public slots:
	void accept();

private:
	inline void addToAutorun();
};

#endif
