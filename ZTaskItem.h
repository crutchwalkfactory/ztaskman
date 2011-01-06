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

#ifndef ZTZTESKITEM_H
#define ZTZTESKITEM_H

#if defined(EZX_Z6) || defined(EZX_V8)
#include <ZListBox.h>
#else
#include <ZSettingItem.h>
#endif

enum EITEMTYPE
{
	ITEM_NATIV,
	ITEM_JAVA,
	ITEM_SPECIAL,
	ITEM_OTHE
};

class Q_EXPORT ZTaskItem : public ZSettingItem
{
public: 
	ZTaskItem( ZListBox* container = 0, const QString itemType = NULL );
	~ZTaskItem();

    void setReservedData(int rsvData);
    int getReservedData();

	void setJava( bool java );
	
	EITEMTYPE getType();
	void setType( EITEMTYPE type);		
	
	uint getPID();
	void setPID( uint pid);
	
	QString getUID();
	void setUID( QString UID);	
	
protected:
	EITEMTYPE type;
	uint pid;
	int data;
	QString uid;
};

#endif
