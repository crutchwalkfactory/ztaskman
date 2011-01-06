//
// Project: zTaskMan
//
// C++ Implementation: 
//
// Description: global defines
//
//

#ifndef ZADDAPP_H
#define ZADDAPP_H

#include "BaseDlg.h"
#include <ZScrollPanel.h>
#include <ZListBox.h>

class AppRegistry
{
public:
    AppRegistry(){}
    ~AppRegistry(){}

	QString id() const { return id_; }
	QString icon() const { return icon_; }
	QString name() const { return name_; }
	void setId( QString salary ) { id_ = salary; }
	void setIcon( QString salary ) { icon_ = salary; }
	void setName( QString salary ) { name_ = salary; }

private:
	QString id_;
	QString name_;
	QString icon_;
};

class ZAddAppDlg : public MyBaseDlg {
  Q_OBJECT

  public:
    ZAddAppDlg(); 
    ~ZAddAppDlg(); 
    ZListBox *lbAppList;
    typedef QMap<int, AppRegistry> mapAppList;
	mapAppList mapApp;
	void addApp(QString name, QString icon, int i);
	void procReg(QString FileRegistry);
      
  public slots:
    virtual void accept();
    virtual void reject();   
};

#endif
