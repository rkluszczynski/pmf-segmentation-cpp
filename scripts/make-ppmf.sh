#!/bin/sh

PROGFILE=../bin/ppmf-sim.exe

WXCFG="/vosoft/unicore/pmf/wx-2.8.12/bin/wx-config"
OBJDIR=`dirname $0`/../objs/src

#Compiling: ..\cmd\SegmentationParameters.cpp
#Compiling: ..\cmd\SynchronizationTimer.cpp
#Compiling: ..\cmd\grayimage.cpp
#Compiling: ..\cmd\image.cpp
#Compiling: ..\cmd\segmentation.cpp
#Compiling: ..\cmd\sim-core.cpp
#Compiling: ..\src\statistics.cpp
#Compiling: ..\cmd\MultiCoreSegmentation.cpp
FILES="../cmd/SegmentationParameters.cpp ../cmd/SynchronizationTimer.cpp ../cmd/grayimage.cpp ../cmd/image.cpp ../cmd/segmentation.cpp ../cmd/sim-core.cpp ../src/statistics.cpp ../cmd/MultiCoreSegmentation.cpp"

CPP="g++ -Wall -fexceptions -fopenmp -I ../src/utils -L ../lib -DDSFMT_MEXP=19937 `$WXCFG --cppflags`"
LFLAGS="-lutils-std `$WXCFG --libs`"

#COMPILE_CMD="g++ -I ../src/utils/ -I ../src/utils/dSFMT -fopenmp -fno-strict-aliasing --param max-inline-insns-single=1800 --param inline-unit-growth=500 --param large-function-growth=900 -DDSFMT_DO_NOT_USE_OLD_NAMES -DDSFMT_MEXP=19937 -Wall -s -O3"

rm $OBJDIR/*.o
for fname in $FILES;
do
	echo "Compiling: $fname"
	bname=`basename ${fname%\.*}`
	$CPP $fname -c -o $OBJDIR/$bname.o
done

$CPP $OBJDIR/*.o -o $PROGFILE  $LFLAGS
ls -al $PROGFILE
