//
// Project: zTaskMan
//
// C++ Interface: MyBaseDlg
//
// Description: 
//
//

#ifndef BASEDLG_H
#define BASEDLG_H

#include <ZKbMainWidget.h>

#ifndef FIX_HEADER
#define FULL_TYPE MAINDISPLAY_HEADER
#endif

class MyBaseDlg : public ZKbMainWidget {
  Q_OBJECT

  public:
    MyBaseDlg(); 
    ~MyBaseDlg();
    
    int exec();
    int result() const { return rescode; }
    
  protected slots:
    virtual void done(int);
    virtual void accept();
    virtual void reject();
        
  private:
    bool myInLoop;
    int rescode;
        
  protected:
    enum DialogCode { Rejected, Accepted };
    void setResult(int r) { rescode = r; }    
};

#endif
