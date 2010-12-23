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

mkdir libMgx
cd libMgx
mkdir $APPNAME

#Gen descriptiom.ini
echo -e "Name = $APPNAME\nExec = $APPNAME\nIcon = $APPNAME.png\nAuthor = Ant-ON\nVersion = $VER\nComment = Task Manager\n" > $APPNAME/$APPNAME.cfg

cp ./../$APPNAME\_$PLATFORM $APPNAME/$APPNAME
cp ./../appicon/zTaskMan1_usr.png $APPNAME/$APPNAME.png
cp ./../script/*.* $APPNAME/
cp ./../script/* $APPNAME/
mkdir $APPNAME/img/
cp ./../img/*.* $APPNAME/img/
mkdir $APPNAME/lng/
cp ./../lng/*.* $APPNAME/lng/
mkdir $APPNAME/panel/
cp ./../panel$PANEL/*.* $APPNAME/panel/

#create PEP
7z a $APPNAME\_$VER_FOR_NAME\_$PLATFORM.mgx $APPNAME/
cp $APPNAME\_$VER_FOR_NAME\_$PLATFORM.mgx ./../pack/
cd ..
rm -r libMgx
