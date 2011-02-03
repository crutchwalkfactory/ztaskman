#
# Project: zTaskMan
#
# Author: Ant-ON <prozanton@gmail.com>, (C) 2008-2011
#

#PLATFORM=EZX-VE66
#PLATFORM=EZX-Z6
#PLATFORM=EZX-Z6W
#PLATFORM=EZX-U9
#PLATFORM=EZX-ZN5
#PLATFORM=EZX-V8
#PLATFORM=EZX-E8
#PLATFORM=EZX-EM30
#PLATFORM=EZX-VA76

######Config######
APPNAME    := zTaskMan
#UPLOAD_PATH :=  /mmc/mmca1/.Programs/zTaskMan/
#UPLOAD_PATH :=  /ezxlocal/download/mystuff/
#UPLOAD_PATH :=  /mmc/mmca1/bin/
UPLOAD_PATH :=  /ezxlocal/download/mystuff/.system/zTaskMan
#FLAGS_CUSTOM = -pg
##################

TOOLPREFIX := /arm-eabi
ARMLIB     := $(TOOLPREFIX)/arm-linux-gnueabi/lib
ZNEWCHECKBOX = 0

ifeq ($(PLATFORM),)
PLATFORM = EZX-Z6
endif

ifeq ($(PLATFORM),EZX-Z6)
QTDIR	:=	$(TOOLPREFIX)/lib/qt-2.3.8
EZXDIR	:=	$(TOOLPREFIX)/lib/ezx-z6
LINKLIB := -lm -lqte-mt -lezxpm -lezxappbase
DIRECTIV := -DEZX_Z6 -DFIX_LISTBOX -DFIX_FORMCONTAINER -DFIX_HEADER -DOLD_PLATFORM -DINDEX_CARD1 -DHARD_KEY_DAEMON -DRAISE_PHONE
TARGET	=       $(APPNAME)
endif

ifeq ($(PLATFORM),EZX-V8)
QTDIR	:=	$(TOOLPREFIX)/lib/qt-v8
EZXDIR	:=	$(TOOLPREFIX)/lib/ezx-v8
LINKLIB := -lm -lqte-mt -lezxpm -lezxappbase 
DIRECTIV := -DEZX_V8 -DFIX_STR_REPLACE -DFIX_FORMCONTAINER -DOLD_PLATFORM -DFIX_HEADER -DINDEX_CARD1 -DRAISE_PHONE
TARGET	=       $(APPNAME)_V8
ZNEWCHECKBOX = 1
endif

ifeq ($(PLATFORM),EZX-E8)
ARMLIB     := $(TOOLPREFIX)/arm-linux-gnueabi/lib_E8
QTDIR	:=	$(TOOLPREFIX)/lib/qt-e8
EZXDIR	:=	$(TOOLPREFIX)/lib/ezx-e8
LINKLIB := -lm -lqte-mt -lezxappbase
DIRECTIV := -DEZX_E8 -DFixByQT -DNEW_PLATFORM -DHARD_KEY_DAEMON -DFIX_MENU_1 -DRAISE_PHONE -DFLOWPLAYER -DGORIZONTAL_SCREEN
TARGET	=       $(APPNAME)_E8
endif

ifeq ($(PLATFORM),EZX-EM30)
ARMLIB     := $(TOOLPREFIX)/arm-linux-gnueabi/lib_E8
QTDIR	:=	$(TOOLPREFIX)/lib/qt-em30
EZXDIR	:=	$(TOOLPREFIX)/lib/ezx-em30
EZXDIR2	:=	$(TOOLPREFIX)/lib/ezx-e8
LINKLIB := -lm -lqte-mt -lezxappbase
DIRECTIV := -DEZX_EM30 -DFixByQT -DNEW_PLATFORM -DHARD_KEY_DAEMON -DRAISE_PHONE -DFLOWPLAYER -DGORIZONTAL_SCREEN
TARGET	=       $(APPNAME)_EM30
endif

ifeq ($(PLATFORM),EZX-VA76)
ARMLIB     := $(TOOLPREFIX)/arm-linux-gnueabi/lib_E8
QTDIR	:=	$(TOOLPREFIX)/lib/qt-e8
EZXDIR	:=	$(TOOLPREFIX)/lib/ezx-va76
LINKLIB := -lm -lqte-mt -lezxappbase
DIRECTIV := -DEZX_VA76 -DFixByQT -DNEW_PLATFORM -DHARD_KEY_DAEMON -DFIX_MENU_1 -DRAISE_PHONE
TARGET	=       $(APPNAME)_VA76
endif

ifeq ($(PLATFORM),EZX-U9)
QTDIR	:=	$(TOOLPREFIX)/lib/qt-zn5
EZXDIR	:=	$(TOOLPREFIX)/lib/ezx-u9
EZXDIR2	:=	$(TOOLPREFIX)/lib/ezx-zn5
LINKLIB := -lm -lqte-mt -lezxappbase
DIRECTIV := -DEZX_U9 -DFixByQT -DFIX_FORMCONTAINER -DFIX_MENU_2 -DRAISE_PHONE -DCUTED_QT
TARGET	=       $(APPNAME)_U9
endif

ifeq ($(PLATFORM),EZX-Z6W)
QTDIR	:=	$(TOOLPREFIX)/lib/qt-z6w
EZXDIR	:=	$(TOOLPREFIX)/lib/ezx-z6w
LINKLIB := -lm -lqte-mt -lezxappbase
DIRECTIV := -DEZX_Z6W -DFixByQT -DFIX_FORMCONTAINER -DFIX_MENU_2 -DRAISE_PHONE -DCUTED_QT
TARGET	=       $(APPNAME)_Z6W
endif

ifeq ($(PLATFORM),EZX-ZN5)
QTDIR	:=	$(TOOLPREFIX)/lib/qt-zn5
EZXDIR	:=	$(TOOLPREFIX)/lib/ezx-zn5
LINKLIB := -lm -lqte-mt -lezxappbase
DIRECTIV := -DEZX_ZN5 -DFixByQT -DFIX_FORMCONTAINER -DHARD_KEY_DAEMON -DFIX_MENU_2 -DRAISE_PHONE -DCUTED_QT
TARGET	=       $(APPNAME)_ZN5
endif

ifeq ($(PLATFORM),EZX-EM35)
ARMLIB     := $(TOOLPREFIX)/arm-linux-gnueabi/lib_E8
QTDIR	:=	$(TOOLPREFIX)/lib/qt-em35
EZXDIR	:=	$(TOOLPREFIX)/lib/ezx-em35
LINKLIB := -lm -lqte-mt -lezxappbase
DIRECTIV := -DEZX_EM35 -DFixByQT -DNEW_PLATFORM -DES_EVENT -DNEW_JAVA_LIST -DHARD_KEY_DAEMON
TARGET	=       $(APPNAME)_EM35
endif

ifeq ($(PLATFORM),EZX-VE66)
ARMLIB     := $(TOOLPREFIX)/arm-linux-gnueabi/lib_E8
QTDIR	:=	$(TOOLPREFIX)/lib/qt-em35
EZXDIR	:=	$(TOOLPREFIX)/lib/ezx-ve66
LINKLIB := -lm -lqte-mt -lezxappbase
DIRECTIV := -DEZX_VE66 -DFixByQT -DNEW_PLATFORM -DES_EVENT -DNEW_JAVA_LIST -DHARD_KEY_DAEMON -DAUTORUN
#TARGET	=       $(APPNAME)_VE66
TARGET	=       $(APPNAME)
UPLOAD_PATH :=  /ezxlocal/download/mystuff/.system/zTaskMan
endif

CC	    =	$(TOOLPREFIX)/bin/arm-linux-gnueabi-gcc
CXX	    =	$(TOOLPREFIX)/bin/arm-linux-gnueabi-g++
LD	    =	$(TOOLPREFIX)/bin/arm-linux-gnueabi-g++
STRIP   =   $(TOOLPREFIX)/bin/arm-linux-strip
CFLAGS	=	-pipe -Wall -W -O2 -DNO_DEBUG $(DIRECTIV) $(INCPATH) -march=armv6j -mtune=arm1136jf-s -Os $(FLAGS_CUSTOM)
CXXFLAGS=	-pipe -DQWS -fno-exceptions -fno-rtti -Wall -W -DNO_DEBUG $(DIRECTIV) $(INCPATH) -march=armv6j -mtune=arm1136jf-s -Os $(FLAGS_CUSTOM) 
INCPATH	=	-I$(QTDIR)/include -I$(EZXDIR)/include -I$(EZXDIR2)/include -I $(TOOLPREFIX)/arm-linux-gnueabi/include
LDFLAGS	=	-s
LINK	=	$(TOOLPREFIX)/bin/arm-linux-gnueabi-gcc
LFLAGS	=	-Wl,-rpath-link,$(EZXDIR)/lib $(FLAGS_CUSTOM)
LIBS	=	$(SUBLIBS) -L$(EZXDIR)/lib -L$(EZXDIR2)/lib -L$(ARMLIB) -L$(QTDIR)/lib $(LINKLIB)
MOC	    =	$(QTDIR)/bin/moc
UIC	    =	$(QTDIR)/bin/uic

####### Files
HEADERS0 =   zTaskMan.h\
            GUI_Define.h\
            BaseDlg.h\
            ZAboutDlg.h\
            ZProcInfo.h\
            ZSettingDlg.h\
            ZAddApp.h\
            lng.h\
            ZTaskItem.h\
            ZLoadSettings.h\
            ZNumSelect.h
ifeq ($(ZNEWCHECKBOX),1)
HEADERS =   $(HEADERS0)\
            ZNewCheckBox.h
else
HEADERS =   $(HEADERS0)
endif
	        
SOURCES0 =   main.cpp\
            zTaskMan.cpp\
            BaseDlg.cpp\
            ZAboutDlg.cpp\
            ZProcInfo.cpp\
            ZSettingDlg.cpp\
            ZAddApp.cpp\
            lng.cpp\
            ZTaskItem.cpp\
            ZLoadSettings.cpp\
            ZNumSelect.cpp\
            ZUtils.cpp
ifeq ($(ZNEWCHECKBOX),1)
SOURCES =   $(SOURCES0)\
            ZNewCheckBox.cpp
else
SOURCES =   $(SOURCES0)
endif

OBJECTS0 =   main.o zTaskMan.o\
            BaseDlg.o\
            ZAboutDlg.o\
            ZProcInfo.o\
            ZSettingDlg.o\
            ZAddApp.o\
            lng.o\
            ZTaskItem.o\
            ZLoadSettings.o\
            ZNumSelect.o\
            ZUtils.o
ifeq ($(ZNEWCHECKBOX),1)
OBJECTS =   $(OBJECTS0)\
            ZNewCheckBox.o
else
OBJECTS =   $(OBJECTS0)
endif

SRCMOC0	=	moc_zTaskMan.cpp\
          moc_BaseDlg.cpp\
          moc_ZAboutDlg.cpp\
          moc_ZProcInfo.cpp\
          moc_ZSettingDlg.cpp\
          moc_ZAddApp.cpp\
          moc_lng.cpp\
          moc_ZTaskItem.cpp\
          moc_ZLoadSettings.cpp\
          moc_ZNumSelect.cpp\
          moc_ZUtils.cpp
ifeq ($(ZNEWCHECKBOX),1)
SRCMOC =   $(SRCMOC0)\
            moc_ZNewCheckBox.h
else
SRCMOC =   $(SRCMOC0)
endif

OBJMOC0	=	moc_zTaskMan.o\
          moc_BaseDlg.o\
          moc_ZAboutDlg.o\
          moc_ZProcInfo.o\
          moc_ZSettingDlg.o\
          moc_ZAddApp.o\
          moc_lng.o\
          moc_ZTaskItem.o\
          moc_ZLoadSettings.o\
          moc_ZNumSelect.o\
          moc_ZUtils.o
ifeq ($(ZNEWCHECKBOX),1)
OBJMOC =   $(OBJMOC0)\
            moc_ZNewCheckBox.o
else
OBJMOC =   $(OBJMOC0)
endif
            
TARGETS = 	$(TARGET)
INTERFACE_DECL_PATH = .

all: $(TARGET)

$(TARGET): $(UICDECLS) $(OBJECTS) $(OBJMOC) 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS)
		$(STRIP) -s $(MAKETO)$(TARGET)
		
moc: $(SRCMOC)

clean:
	-rm -f $(OBJECTS) $(OBJMOC) $(SRCMOC) $(UICIMPLS) $(UICDECLS)
	-rm -f *~ core
	$(RM) *.o
	$(RM) moc_*.cpp

up:
	hg ci -m "Auto UP by MakeFile"
	hg push

#--------- moc's -----------------------------
moc_zTaskMan.o: moc_zTaskMan.cpp zTaskMan.h
moc_zTaskMan.cpp: zTaskMan.h
	$(MOC) zTaskMan.h -o moc_zTaskMan.cpp
  
moc_BaseDlg.o: moc_BaseDlg.cpp BaseDlg.h
moc_BaseDlg.cpp: BaseDlg.h
	$(MOC) BaseDlg.h -o moc_BaseDlg.cpp

moc_ZAboutDlg.o: moc_ZAboutDlg.cpp ZAboutDlg.h
moc_ZAboutDlg.cpp: ZAboutDlg.h
	$(MOC) ZAboutDlg.h -o moc_ZAboutDlg.cpp
	
moc_ZProcInfo.o: moc_ZProcInfo.cpp ZProcInfo.h
moc_ZProcInfo.cpp: ZProcInfo.h
	$(MOC) ZProcInfo.h -o moc_ZProcInfo.cpp
	
moc_ZSettingDlg.o: moc_ZSettingDlg.cpp ZSettingDlg.h
moc_ZSettingDlg.cpp: ZSettingDlg.h
	$(MOC) ZSettingDlg.h -o moc_ZSettingDlg.cpp	
	
moc_ZAddApp.o: moc_ZAddApp.cpp ZAddApp.h
moc_ZAddApp.cpp: ZAddApp.h
	$(MOC) ZAddApp.h -o moc_ZAddApp.cpp	
		
moc_lng.o: moc_lng.cpp lng.h
moc_lng.cpp: lng.h
	$(MOC) lng.h -o moc_lng.cpp	

moc_ZTaskItem.o: moc_ZTaskItem.cpp ZTaskItem.h
moc_ZTaskItem.cpp: ZTaskItem.h
	$(MOC) ZTaskItem.h -o moc_ZTaskItem.cpp
		
moc_ZLoadSettings.o: moc_ZLoadSettings.cpp ZLoadSettings.h
moc_ZLoadSettings.cpp: ZLoadSettings.h
	$(MOC) ZLoadSettings.h -o moc_ZLoadSettings.cpp		
	
moc_ZNumSelect.o: moc_ZNumSelect.cpp ZNumSelect.h
moc_ZNumSelect.cpp: ZNumSelect.h
	$(MOC) ZNumSelect.h -o moc_ZNumSelect.cpp		

moc_ZUtils.o: moc_ZNumSelect.cpp ZNumSelect.h
moc_ZUtils.cpp: ZUtils.cpp
	$(MOC) ZUtils.cpp -o moc_ZUtils.cpp

ifeq ($(ZNEWCHECKBOX),1)
moc_ZNewCheckBox.o: moc_ZNewCheckBox.cpp ZNewCheckBox.h
moc_ZNewCheckBox.cpp: ZNewCheckBox.h
	$(MOC) ZNewCheckBox.h -o moc_ZNewCheckBox.cpp
endif
			
#----------------------------------------------
install: $(TARGET)
	ncftpput -u root -p "" 192.168.16.2 $(UPLOAD_PATH) $(TARGET)

pep: $(TARGET)
	./MakePEP.sh $(PLATFORM)
	
mgx: $(TARGET)
	./MakeMGX.sh $(PLATFORM)

