//
// Project: zTaskMan
//
// C++ Implementation: ZTaskItem
//
// Description: item of task list
//
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2010-2011
//

#include "ZTaskItem.h"

ZTaskItem::ZTaskItem( ZListBox* container, const QString itemType ):
	ZSettingItem( container, itemType )
{
	type = ITEM_NATIV;
	pid = 0;
	data = 0;
}

ZTaskItem::~ZTaskItem()
{
}

void ZTaskItem::setReservedData(int rsvData)
{
	data = rsvData;
}

int ZTaskItem::getReservedData()
{
	return data;
}

void ZTaskItem::setJava( bool java )
{
	if ( java )
		setType(ITEM_JAVA);
	else
		setType(ITEM_NATIV);
}


EITEMTYPE ZTaskItem::getType()
{
	return type;
}

void ZTaskItem::setType( EITEMTYPE t)
{
	type = t;
}

uint ZTaskItem::getPID()
{
	return pid;
}

void ZTaskItem::setPID( uint pid2)
{
	pid = pid2;
}

QString ZTaskItem::getUID()
{
	return uid;		
}

void ZTaskItem::setUID( QString UID)
{
	uid = UID;
}
