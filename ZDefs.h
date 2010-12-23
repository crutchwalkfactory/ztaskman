#ifndef ZDEFS
#define ZDEFS 

// Sys config / chenel
#define SYS_REGISTRY "/usr/data_resource/preloadapps/SysRegistry"
#define CARD_REGISTRY "/mmc/mmca1/.system/java/CardRegistry"
#define USER_REGISTRY "/ezxlocal/download/appwrite/am/UserRegistry"
#define INSTALLED_DB "/ezxlocal/download/appwrite/am/InstalledDB"

#define EZXQTSERVER_CHANNEL "EZX/QtServer"
#define SYSTEM_CHANNEL "EZX/System"
#define SYS_SETUP_CHANNEL "/systemsetup/broadcast"
#define launcherChannel     "/AM/Launcher"

#define MSG_SLIDER_OPENED "sliderOpened"
#define MSG_SLIDER_CLOSED "sliderClosed"

#define APP_CHANNEL_PREF "EZX/Application/"

#define HARD_KEY_CHENEL "/hardkey/bc"

//
#define RETURN_IDLE_EVENT            5678
#define RETURN_IDLE_BY_END_KEY       -5000
#define RETURN_IDLE_BY_FLIP          -5001
#define RETURN_IDLE_BY_APP_TIMEOUT   -5002
#define RETURN_IDLE_BY_SLIDER        -5003
//Need patch libezxappbase.so
#define RETURN_IDLE_BY_ZTASKMAN      0

//Define key
#define KEY_LEFT              0x1012
#define KEY_UP                0x1013
#define KEY_RIGHT             0x1014
#define KEY_DOWN              0x1015
#define KEY_CENTER            0x1004

#define KEY_SIDE_SELECT       0x1005

#define KEY_SIDE_UP           0x1016
#define KEY_SIDE_DOWN         0x1017

#define KEY_GREEN             0x1030
#define KEY_RED               0x1031
#define KEY_C                 0x1032
#define KEY_LEFT_SOFTKEY      0x1038
#define KEY_RIGHT_SOFTKEY     0x103a
#define KEY_MUSIC             0x1048

#define KEY_POWER             0x103f
#define KEY_VR                0x1035

#ifdef EZX_Z6
#define KEY_MSG_PRESS         1
#define KEY_MSG_RELEASE       2
#define KEY_MSG_HOLD          4
#else
#include <ZKeyDef.h>
#endif

// User config / chenel
#define appConf "/ezxlocal/download/appwrite/setup/pe_taskman.cfg"
#define appConfName "/PE_zTaskMan.ru.cfg"

#define ZTASKMAN_START "PE/zTaskMan/"

#define SDL_LIB_CHENEL "PE/SDLLib/"

#define LNG_PATH ProgDir+"/lng/"

// screen size
#ifndef EZX_E8
#define SCREEN_WHIDTH 240
#define SCREEN_HEIGHT 320
#else
#define SCREEN_WHIDTH 320
#define SCREEN_HEIGHT 240
#endif

#define PANEL_WHIDTH 55

#ifndef EZX_E8
#define GRAPH_LEN 10
#define GRAPH_WIDTH 200
#define GRAPH_HEIGHT 50
#else
#define GRAPH_LEN 10
#define GRAPH_WIDTH 145
#define GRAPH_HEIGHT 60
#endif

#define INC_GRAPH(p) ((p<GRAPH_LEN-1)?(p++):(p=0))
#define DEC_EX_GRAPH(p) ((p>1)?(p-1):(GRAPH_LEN-1))

#endif
