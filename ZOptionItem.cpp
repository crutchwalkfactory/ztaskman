//
// Project: zTaskMan
//
// C++ Implementation: ZOptionItem
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2010-2011
//

#include "ZOptionItem.h"

#include <ZPopup.h>
#include <ZNumPickerDlg.h>
#include <ZSingleSelectDlg.h>
#include <ZSingleCaptureDlg.h>
#include <ZColorPickerDlg.h>

ZOptionItem::ZOptionItem( ZListBox* _container, EDIT_TYPE _type, int id ):
	ZSettingItem( _container, "%M" )
{
	this->id = id;
	type=_type;
	n=0;
	
	min=0;
	max=0;
	title="";
	text="";
	cfgName="";
	list=NULL;
	changed=false;

	appendSubItem(0, " ", true);
	appendSubItem(0, " ");
	
	connect( _container, SIGNAL(selected(ZSettingItem*)), this, SLOT(selected(ZSettingItem*)));
}

ZOptionItem::~ZOptionItem()
{
	disconnect( listBox(), SIGNAL(selected(ZSettingItem*)), this, SLOT(selected(ZSettingItem*)));
	if ( list != NULL )
		delete list;
}

void ZOptionItem::selected(ZSettingItem* item)
{
	if ( (ZOptionItem*)item != this )
		return;
	
	switch ( type )
	{
		case EDIT_TEXT:
			{
			ZSingleCaptureDlg* zscd = new ZSingleCaptureDlg(title, "", ZSingleCaptureDlg::TypeLineEdit, this, "", true, 0, 0);
			ZLineEdit* zle = (ZLineEdit*)zscd->getLineEdit();
			zle->setText(text);
			if ( zscd->exec() == QDialog::Accepted )
			{
				setText(zle->text());
				changed=true;
			}
			delete zle;
			delete zscd;
			}
			break;
		case EDIT_BOOL_YESNO:
		case EDIT_BOOL_ONOFF:
			{
			QStringList itemList;
			if ( type==EDIT_BOOL_ONOFF )
			{
				itemList.append(QObject::tr("TXT_RID_SETTING_B_ON", "On"));
				itemList.append(QObject::tr("TXT_RID_SETTING_B_OFF", "Off"));
			} else
			{
				itemList.append(QObject::tr("TXT_RID_CLI_SOFTKEY_YES", "Yes"));
				itemList.append(QObject::tr("TXT_RID_CLI_SOFTKEY_NO", "No"));				
			}
			ZSingleSelectDlg *dlg = new ZSingleSelectDlg(title, "", this);
			dlg->addItemsList(itemList);
			dlg->getListBox()->checkItem(!n, true);
			if ( dlg->exec() == QDialog::Accepted )
			{
				setNum(!dlg->getCheckedItemIndex());
				changed=true;
			}
			delete dlg;
			}
			break;
		case EDIT_NUM:
			{
			ZNumPickerDlg * dlg = new ZNumPickerDlg(2, this);
			ZNumModule* num = dlg->getNumModule();
			num->setMaxValue(max);
			num->setMinValue(min);
			num->setValue(n);
			if ( dlg->exec() == QDialog::Accepted )
			{
				setNum( num->getValue() );
				changed=true;
			}
			delete num;
			delete dlg;
			}
			break;
		case EDIT_ONE_OF_LIST:
			{
			if ( list == NULL )
				return;
			ZSingleSelectDlg *dlg = new ZSingleSelectDlg(title, "", this);
			dlg->addItemsList(*list);
			dlg->getListBox()->checkItem(n, true);
			if ( dlg->exec() == QDialog::Accepted )
			{
				setNum(dlg->getCheckedItemIndex());
				changed=true;
			}
			delete dlg;	
			}	
			break;
		#ifndef NO_CHANGE_COLOR	
		case EDIT_COLOR:
			{
			QColor color;
			color.setNamedColor(text);
			
			ZColorPickerDlg * dlg = new ZColorPickerDlg(color,NULL,0,this);
			if ( dlg->exec() == QDialog::Accepted )
			{
				setText(dlg->getColor().name());
				changed=true;
			}
			delete dlg;	
			}	
			break;
		#endif		
	}
}

void ZOptionItem::setNum(int _n)
{
	n=_n;
	switch ( type )
	{
		case EDIT_BOOL_YESNO:
			setText(n?QObject::tr("TXT_RID_CLI_SOFTKEY_YES", "Yes"):QObject::tr("TXT_RID_CLI_SOFTKEY_NO", "No"));
			break;
		case EDIT_BOOL_ONOFF:
			setText(n?QObject::tr("TXT_RID_SETTING_B_ON", "On"):QObject::tr("TXT_RID_SETTING_B_OFF", "Off"));
			break;
		case EDIT_NUM:
			setText(QString::number(n));
			break;
		case EDIT_ONE_OF_LIST:
			if ( list != NULL )
				setText(*(list->at(n)));
			break;		
		default:
			break;
	}
}

void ZOptionItem::setMaxMin(int _max, int _min)
{
	max=_max;
	min=_min;
}

int ZOptionItem::getNum()
{
	return n;
}

void ZOptionItem::setTitle(QString _title)
{
	title=_title;
	setSubItem(0, 0, title, true);	
}
	
void ZOptionItem::setText(QString _text)
{
	text=_text;
	setSubItem(0, 1, text);
}

QString ZOptionItem::getText()
{
	return text;
}

void ZOptionItem::setList( QStringList * _list )
{
	list = _list;
}

void ZOptionItem::read(QString name, QString section, QString param, QString def)
{
	cfgName = name;
	cfgSection = section;
	cfgParam = param;
	
	ZConfig cfg(cfgName);
	
	switch ( type )
	{
		case EDIT_TEXT:
		case EDIT_COLOR:
			setText( cfg.readEntry(cfgSection, cfgParam, def) );
			break;
		default:
			break;
	}
}

void ZOptionItem::read(QString name, QString section, QString param, int def)
{
	cfgName = name;
	cfgSection = section;
	cfgParam = param;
	
	ZConfig cfg(cfgName);
	switch ( type )
	{
		case EDIT_BOOL_YESNO:
		case EDIT_BOOL_ONOFF:		
			setNum( cfg.readBoolEntry(cfgSection, cfgParam, def) );
			break;			
		case EDIT_NUM:			
		case EDIT_ONE_OF_LIST:
			setNum( cfg.readNumEntry(cfgSection, cfgParam, def) );
			break;	
		default:
			break;
	}
}

void ZOptionItem::write()
{
	if ( !changed )
		return;
	
	if ( !cfgName.isEmpty() )
	{
		ZConfig cfg(cfgName);
		switch ( type )
		{
			case EDIT_TEXT:
			case EDIT_COLOR:
				cfg.writeEntry(cfgSection, cfgParam, getText());
				break;
			case EDIT_BOOL_YESNO:
			case EDIT_BOOL_ONOFF:		
				cfg.writeEntry(cfgSection, cfgParam, (bool)getNum());
				break;			
			case EDIT_NUM:			
			case EDIT_ONE_OF_LIST:
				cfg.writeEntry(cfgSection, cfgParam, getNum());
				break;
			default:
				break;
		}	
		cfg.flush();
	}
	
	emit aplly(id);
}

int ZOptionItem::getId()
{
	return id;
}
