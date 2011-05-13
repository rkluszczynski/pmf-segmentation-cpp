#!/bin/sh

LIBFILE=../lib/libdetect_d.a

OBJDIR=`dirname $0`/../objs/Debug/detect

#Compiling: ..\detect\DetectorSweepLine.cpp
#Compiling: ..\detect\IntersectionDetector.cpp
#Compiling: ..\detect\ScheduleEvent.cpp
#Compiling: ..\detect\SweepLineElement.cpp
#Compiling: ..\detect\DetectorSchedule.cpp
FILES="../detect/DetectorSweepLine.cpp ../detect/IntersectionDetector.cpp ../detect/ScheduleEvent.cpp ../detect/SweepLineElement.cpp ../detect/DetectorSchedule.cpp"

CPP="g++ -Wall"

#COMPILE_CMD="g++ -I ../src/utils/ -I ../src/utils/dSFMT -fopenmp -fno-strict-aliasing --param max-inline-insns-single=1800 --param inline-unit-growth=500 --param large-function-growth=900 -DDSFMT_DO_NOT_USE_OLD_NAMES -DDSFMT_MEXP=19937 -Wall -s -O3"

mkdir -p $OBJDIR
rm -f $OBJDIR/*.o
for fname in $FILES;
do
	echo "Compiling: $fname"
	bname=`basename ${fname%\.*}`
	$CPP $fname -c -o $OBJDIR/$bname.o
done

rm -f $LIBFILE
echo "Linking static library: $LIBFILE"
ar -cvq $LIBFILE $OBJDIR/*.o
ls -al $LIBFILE

