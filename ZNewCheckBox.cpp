//
// Project: zTaskMan
//
// C++ Implementation: ZNewCheckBox
//
// Description: fix ZCheckBox on Motorola Z6/V8  
//
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2009
//

#include "ZNewCheckBox.h"

ZNewCheckBox::ZNewCheckBox( const QString & text, QWidget * parent) : ZCheckBox ( text, parent, 0 )
{
	setChecked(false);
    connect (this,SIGNAL(toggled(bool)),this,SLOT(slotSChanged(bool)));
}

ZNewCheckBox::~ZNewCheckBox()
{
};
   

bool ZNewCheckBox::isChecked()
{
	return curState;
};

void ZNewCheckBox::setChecked( bool check )
{
	curState = check;
	if (check)
	{
	    setState((ZBaseButton::ToggleState)2);
	}
	else
	{
	    setState((ZBaseButton::ToggleState)0);
	}
};


void ZNewCheckBox::slotSChanged(bool state)
{
	curState = state;
}
