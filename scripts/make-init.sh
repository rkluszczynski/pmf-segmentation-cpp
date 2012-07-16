#!/bin/sh

PROGFILE=../bin/init.exe

. $(dirname $0)/plg-env.cfg

OBJDIR=`dirname $0`/../objs/init

#Compiling: ..\cmd\grayimage.cpp
#Compiling: ..\cmd\image.cpp
#Compiling: ..\init\AbstractProperties.cpp
#Compiling: ..\init\GaussianRandomClass.cpp
#Compiling: ..\init\MosaicConstants.cpp
#Compiling: ..\init\MosaicDualGraph.cpp
#Compiling: ..\init\MosaicGraph.cpp
#Compiling: ..\init\MosaicGraphEdge.cpp
#Compiling: ..\init\MosaicGraphNode.cpp
#Compiling: ..\init\MosaicLinesDeterminer.cpp
#Compiling: ..\init\MosaicPMF.cpp
#Compiling: ..\init\main.cpp
FILES="../cmd/grayimage.cpp ../cmd/image.cpp ../init/AbstractProperties.cpp ../init/GaussianRandomClass.cpp ../init/MosaicConstants.cpp ../init/MosaicDualGraph.cpp ../init/MosaicGraph.cpp ../init/MosaicGraphEdge.cpp ../init/MosaicGraphNode.cpp ../init/MosaicLinesDeterminer.cpp ../init/MosaicPMF.cpp ../init/main.cpp"

CPP="g++ -O2 -Wall -fexceptions -I ../detect -I ../src/utils -L ../lib `${WXCFG} --cppflags`"
LFLAGS="-lutils-std -ldetect_d `${WXCFG} --libs`"

#COMPILE_CMD="g++ -I ../src/utils/ -I ../src/utils/dSFMT -fopenmp -fno-strict-aliasing --param max-inline-insns-single=1800 --param inline-unit-growth=500 --param large-function-growth=900 -DDSFMT_DO_NOT_USE_OLD_NAMES -DDSFMT_MEXP=19937 -Wall -s -O3"

mkdir -p $OBJDIR
rm -f $OBJDIR/*.o
for fname in $FILES;
do
	echo "Compiling: $fname"
	bname=`basename ${fname%\.*}`
	$CPP $fname -c -o $OBJDIR/$bname.o
done

rm -f $PROGFILE
echo "Linking console executable: $PROGFILE"
$CPP $OBJDIR/*.o -o $PROGFILE  $LFLAGS
ls -al $PROGFILE

