//
// Project: zTaskMan
//

#ifndef _H_GUI_DEFINE_H
#define _H_GUI_DEFINE_H

#ifndef __cplusplus
#error "This is a C++ header file; it requires C++ to compile."
#endif

//--------------------------------------------------------------------------------------------------
//
//   Header Name: GUI_Define.h
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------

#include <qwidget.h>
#include <ZApplication.h>
#include "ZGlobal.h"

#define FRAME_WIDTH 2
#define MARGIN_SPACE 5

#define LINE_HEIGHT ZGlobal::getLineHeight()
#define LINE_SPACE  ZGlobal::getLineSpace()

#define LEFT_X_SCROLL MARGIN_SPACE
#define LEFT_X MARGIN_SPACE+FRAME_WIDTH
#define TOP_Y 5
#define TOP_Y_WITH_TITLEBAR 26

#ifndef SET_MAIN_WIDGET
#define SET_MAIN_WIDGET(pWgt, Title) 		ZApplication* appSet = (ZApplication*)qApp;\
							                		appSet->setMainWidget(pWgt);
#endif

class GUIObjFilterClose : public QObject
{
        bool eventFilter(QObject *pObj, QEvent *pEvent)
        {
                if( pEvent->type() == QEvent::Close )
                {
                        delete this;
                        delete pObj;
                        return true;
                }

                return false;
        }
};
#ifndef DELETE_WIDGET_LATER
#define DELETE_WIDGET_LATER(pWgt)       GUIObjFilterClose *pFilter = new GUIObjFilterClose; \
                                        pWgt->installEventFilter( pFilter );\
                                        QApplication::postEvent( pWgt, new QCloseEvent );\
										if(qApp){qApp->wakeUpGuiThread(); }
#endif 

#ifndef MSG_ERR_ICON
#define MSG_ERR_ICON     "Dialog_Error.gif"
#endif

#ifndef MSG_OK_ICON
#define MSG_OK_ICON	"Dialog_Complete.gif"
#endif

#ifndef MSG_QUESTION_ICON
#define MSG_QUESTION_ICON "Dialog_Question_Mark.gif"
#endif

#ifndef MSG_WAIT_ICON
#define MSG_WAIT_ICON    "Dialog_Waiting.gif"
#endif

#endif //_H_SETUP_UI_DEFINE_H
