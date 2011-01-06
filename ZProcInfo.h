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

#ifndef ZPROCINFO_H
#define ZPROCINFO_H

#include "BaseDlg.h"
#include <ZNavTabWidget.h>
#include <ZFormContainer.h>
#include <ZListBox.h>

class ZProcInfoDlg : public MyBaseDlg 
{
	Q_OBJECT

public:
	ZProcInfoDlg( int pid );
	~ZProcInfoDlg();	 

public slots:
    void slotPageChanged(QWidget* AWidget);
	
protected:
	QString getStatus(int pid, QString param, bool showName=true);

private:
	ZNavTabWidget * tabWidget;  
	ZFormContainer *form;
	ZListBox * usesLib;

protected:
    bool  eventFilter( QObject *, QEvent * );    
};

#endif
