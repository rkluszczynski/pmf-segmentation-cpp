#!/bin/sh

LIBFILE=../lib/libutils-std.a
OBJDIR=`dirname $0`/../objs/utils

#Compiling: ..\src\utils\NearestPointsDistance.cpp
#Compiling: ..\src\utils\NumericalParameters.cpp
#Compiling: ..\src\utils\PrallelDoublePRNG.cpp
#Compiling: ..\src\utils\SharedDoublePRNG.cpp
#Compiling: ..\src\utils\_NumericParameters.cpp
#Compiling: ..\src\utils\wrapper.c
#Compiling: ..\src\utils\DoublePRNG.cpp
FILES="../src/utils/NearestPointsDistance.cpp ../src/utils/NumericalParameters.cpp ../src/utils/PrallelDoublePRNG.cpp ../src/utils/SharedDoublePRNG.cpp ../src/utils/_NumericParameters.cpp ../src/utils/wrapper.c ../src/utils/DoublePRNG.cpp"

CPP="g++ -I ../src/utils/ -I ../src/utils/dSFMT -fopenmp -fno-strict-aliasing --param max-inline-insns-single=1800 --param inline-unit-growth=500 --param large-function-growth=900 -DDSFMT_DO_NOT_USE_OLD_NAMES -DDSFMT_MEXP=19937 -Wall -s -O3"

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
#ar -t ..\lib\libutils-std.a

#g++ src/utils/DoublePRNG.cpp -I src/utils/ -fopenmp -fno-strict-aliasing --param max-inline-insns-single=1800 --param inline-unit-growth=500 --param large-function-growth=900 -DDSFMT_DO_NOT_USE_OLD_NAMES -DDSFMT_MEXP=19937 -Wall -s -O3 -c $FILE -o `basename $FILE`

