#/bin/sh

#Info
export VER=`cat lng.h | grep LNG_VER | awk '{ print $3 }' | sed 's/"//g'`
export VER_FOR_NAME=`echo $VER | sed 's/\./_/g'`
export PLATFORM=$1
export APPNAME=$2
export PANEL=

if [ `echo $PLATFORM | grep EZX` ]
then
export PLATFORM=`echo $PLATFORM | sed 's/EZX-//g'`
fi
if [ "$PLATFORM" = "E8" ]
then
export PANEL=_E8
else
if [ "$PLATFORM" = "EM30" ]
then
export PANEL=_E8
fi
fi

mkdir libPep
cd libPep
mkdir app

#Gen descriptiom.ini
echo -e "[Actions]\nSilent=0\nInstallApp=1\nAddAppToMenu=1\n\n[InstallApp]\nName=$APPNAME\nExec=$APPNAME\nInstallPath=../../mmc/mmca1/.Programs/$APPNAME/\nAppLockable=0\nAuthor=Ant-ON\nVersion=$VER\nDaemon=1\nBigIcon=zTaskMan3_usr.png\nAniIcon="$APPNAME"1_usr.png\nIcon="$APPNAME"2_usr.png\n\n[AddAppToMenu]\nParentFolder=root\n" > description.ini

if [ "$PLATFORM" = "Z6" ]
then
cp ./../$APPNAME app/$APPNAME
else
cp ./../$APPNAME\_$PLATFORM app/$APPNAME
fi
cp ./../appicon/*.* app/
cp ./../script/*.* app/
cp ./../script/* app/
mkdir app/img/
cp ./../img/*.* app/img/
mkdir app/lng/
cp ./../lng/*.* app/lng/
mkdir app/panel/
cp ./../panel$PANEL/*.* app/panel/

#create PEP
tar -czf $APPNAME\_$VER_FOR_NAME\_$PLATFORM.pep --gzip app/ description.ini

cp $APPNAME\_$VER_FOR_NAME\_$PLATFORM.pep ./../pack/
cd ..
rm -r libPep
