//
// Project: zTaskMan
//
// C++ Implementation: ZOptionItem
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2010-2011
//

#ifndef ZOPTIONITEM_H
#define ZOPTIONITEM_H

#ifdef OLD_PLATFORM
#include <ZListBox.h>
#else
#include <ZSettingItem.h>
#endif

#include <qstringlist.h>
#include <qcolor.h>

class Q_EXPORT ZOptionItem: public QWidget, public ZSettingItem
{
	Q_OBJECT

public: 
	enum EDIT_TYPE
	{
		EDIT_TEXT,	
		EDIT_BOOL_YESNO,
		EDIT_BOOL_ONOFF,		
		EDIT_NUM,
		EDIT_ONE_OF_LIST,
		EDIT_COLOR,				
	};

	ZOptionItem( ZListBox* _container, EDIT_TYPE _type, int id );
	~ZOptionItem();
	
	void setTitle(QString _title);
	
	void setText(QString _text);
	QString getText();
	
	void setNum(int _n);
	void setMaxMin(int _max, int _min=0);
	int  getNum();
	
	void setList( QStringList * _list );

	void read(QString name, QString section, QString param, QString def);
	void read(QString name, QString section, QString param, int def);
	void write();
	
	int getId();
	
protected slots:
	void selected(ZSettingItem*);

signals:
	void aplly( int );

private:
	EDIT_TYPE type;
	int n, min, max;
	QString title, text;
	QStringList * list;
	bool changed;
	int id;
	
	QString cfgName, cfgSection, cfgParam;
};

#endif
