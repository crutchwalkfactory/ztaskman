#ifndef ZNEWCHECKBOX_H
#define ZNEWCHECKBOX_H

#include <ZCheckBox.h>

class ZNewCheckBox : public ZCheckBox 
{
  Q_OBJECT

  public:
    ZNewCheckBox( const QString & text, QWidget * parent);
    ~ZNewCheckBox();   
   
    bool isChecked();

    void setChecked( bool check );
    
  private slots:
    void slotSChanged(bool state);
    
  private:
    bool curState; 

};

#endif
