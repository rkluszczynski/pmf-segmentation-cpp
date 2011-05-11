#!/bin/sh

#Compiling: ..\src\utils\NearestPointsDistance.cpp
#Compiling: ..\src\utils\NumericalParameters.cpp
#Compiling: ..\src\utils\PrallelDoublePRNG.cpp
#Compiling: ..\src\utils\SharedDoublePRNG.cpp
#Compiling: ..\src\utils\_NumericParameters.cpp
#Compiling: ..\src\utils\wrapper.c
#Compiling: ..\src\utils\DoublePRNG.cpp
FILES="NearestPointsDistance.cpp NumericalParameters.cpp PrallelDoublePRNG.cpp SharedDoublePRNG.cpp _NumericParameters.cpp wrapper.c DoublePRNG.cpp"

COMPILE_CMD="g++ -I ../src/utils/ -I ../src/utils/dSFMT -fopenmp -fno-strict-aliasing --param max-inline-insns-single=1800 --param inline-unit-growth=500 --param large-function-growth=900 -DDSFMT_DO_NOT_USE_OLD_NAMES -DDSFMT_MEXP=19937 -Wall -s -O3"

for fname in $FILES;
do
	echo "Compiling: ../src/utils/$fname"
	bname=`echo $fname | cut -d\. -f1`
	$COMPILE_CMD -c ../src/utils/$fname -o ../objs/Debug/src/utils/$bname.o
done

ar -cvq ../lib/libutils-std.a ../objs/Debug/src/utils/*.o
ls -al ../lib/libutils-std.a
#ar -t ..\lib\libutils-std.a

#g++ src/utils/DoublePRNG.cpp -I src/utils/ -fopenmp -fno-strict-aliasing --param max-inline-insns-single=1800 --param inline-unit-growth=500 --param large-function-growth=900 -DDSFMT_DO_NOT_USE_OLD_NAMES -DDSFMT_MEXP=19937 -Wall -s -O3 -c $FILE -o `basename $FILE`

