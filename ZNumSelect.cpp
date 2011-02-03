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

#include "ZNumSelect.h"
#include "lng.h"

extern ZLng* lng;

ZNumSelect::ZNumSelect(QWidget * perent, QString preText)
	: ZPopup( ZPopup::DIALOG, perent )
{
	text = preText;
	smPrior = new ZSliderModule(-10,10,1,0,ZSlider::NULL_MINMAX, this);
	lbText = new ZLabel(this);
	setNum(0);
	
	
	insertChild(lbText);
	insertChild(smPrior);
	setSoftkeyTextLeft(lng->getString("OK"));
	setSoftkeyTextRight(lng->getString("CANCEL"));	
	setTitle(" ");
	
	connect ( smPrior, SIGNAL ( valueChanged(int) ), this, SLOT ( valueChanged(int) ) );
}

ZNumSelect::~ZNumSelect()
{
	
}

void ZNumSelect::setNum(int num)
{
	iNum = num;
	lbText->setText( text + " " + QString::number(iNum)  );
	smPrior->getSlider()->setValue(iNum);
}

int ZNumSelect::getNum()
{
	return iNum;
}

void ZNumSelect::valueChanged(int num)
{
	iNum = num;
	lbText->setText( text + " " + QString::number(iNum)  );
}

