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

mkdir libMpkg
cd libMpkg

#Gen desktop file
echo -e "[Desktop Entry]\nName = $APPNAME $VER by Ant-ON\nDirectory = $APPNAME\nExec = $APPNAME\nBigIcon = "$APPNAME"3_usr.png\nAniIcon = "$APPNAME"1_usr.png\n" > $APPNAME\_$VER_FOR_NAME\_$PLATFORM.desktop


if [ "$PLATFORM" = "Z6" ]
then
cp ./../$APPNAME $APPNAME
else
cp ./../$APPNAME\_$PLATFORM $APPNAME
fi
cp ./../appicon/*.* ./
cp ./../script/*.* ./
cp ./../script/* ./
mkdir img
cp ./../img/*.* img/
mkdir lng
cp ./../lng/*.* lng/
mkdir panel
cp ./../panel$PANEL/*.* panel/

#create MPKG
tar -czf $APPNAME\_$VER_FOR_NAME\_$PLATFORM.mpkg --gzip *

cp $APPNAME\_$VER_FOR_NAME\_$PLATFORM.mpkg ./../pack/
cd ..
rm -r libMpkg
