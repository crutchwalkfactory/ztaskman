#/bin/sh

export APPNAME=zTaskMan

for p in Z6 ZN5 U9 V8
do

if [ "$1" = "new" ]
then
make clean
make PLATFORM=EZX-$p
fi

./MakePEP.sh $p $APPNAME

done

for p in E8 ZN5 U9 EM35 EM30
do

if [ "$1" = "new" ]
then
make clean
make PLATFORM=EZX-$p
fi

./MakeMGX.sh $p $APPNAME

done

for p in Z6 V8
do

if [ "$1" = "new" ]
then
make clean
make PLATFORM=EZX-$p
fi

./MakeMPKG.sh $p $APPNAME

done
