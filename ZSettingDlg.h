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
#include <ZCheckBox.h>
#include <ZComboBox.h>
#include <ZPressButton.h>
#include <ZLabel.h>
#include <ZNumPickerDlg.h>
#if defined(EZX_ZN5) || defined(EZX_U9) || defined(EZX_Z6W)
#include <ZColorPickerDlg.h>
#else
#include <ZColorControl.h>
#endif

#ifdef EZX_V8
#include "ZNewCheckBox.h"
#define ZCheckBox ZNewCheckBox
#endif

class ZAppSetting : public MyBaseDlg 
{
  Q_OBJECT

  public:
    ZAppSetting();
    ~ZAppSetting();

    ZCheckBox *zcbFiltrProc;
    ZCheckBox *zcbTaskNoWin;
    
    #ifdef RAISE_PHONE
    ZCheckBox *zcbSendReasePhone;
    ZCheckBox *zcbSendGoToIDLE;
    #endif
    
    ZCheckBox *zcbKeyGrren;
    ZCheckBox *zcbKeyC;    

    ZLabel *zlLang;
    ZComboBox *zcbLanguage;
        
    ZLabel *zlFont;	
	ZPressButton *zpbFontSize;
	
    ZLabel *zlPanelFont;	
	ZPressButton *zpbPanelFontSize;	
	
	#if defined(EZX_ZN5) || defined(EZX_U9) || defined(EZX_Z6W)
	ZPressButton *zpbFontColor;	
	QColor fontColor;
	#else
	ZColorControl * zccFontColor;
	#endif
	
	ZCheckBox *zcbCustomFontColor;
	ZCheckBox *zcbTimeInCaption;

	int fontSize;
	int fontPanelSize;	
    
  public slots:
    virtual void accept();
    virtual void reject();
	void changeFont();
	void changePanelFont();
	void changeFontColor();
};

#endif
