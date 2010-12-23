//
// C++ Implementation: ZNumSelect
//
// Description:
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2010
//

#ifndef ZNUMSELECT_H
#define ZNUMSELECT_H

//EZX
#include <ZPopup.h>
#if defined(EZX_Z6)||defined(EZX_V8)
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
