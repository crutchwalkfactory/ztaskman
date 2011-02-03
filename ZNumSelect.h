//
// Project: zTaskMan
//
// C++ Implementation: ZNumSelect
//
// Description: select number dlg
//
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2010-2011
//

#ifndef ZNUMSELECT_H
#define ZNUMSELECT_H

//EZX
#include <ZPopup.h>
#ifdef OLD_PLATFORM
#include <ZSlider.h>
#else
#include <ZSliderModule.h>
#endif

class ZNumSelect : public ZPopup
{
	Q_OBJECT
	
	public:
		ZNumSelect(QWidget * perent, QString preText); 
		~ZNumSelect(); 
		void setNum(int);
		int getNum();
	
	public slots:
		void valueChanged(int);
	
	private:
		ZSliderModule * smPrior;
		QString text;
		int iNum;
		ZLabel * lbText;
};

#endif
