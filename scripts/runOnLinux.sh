#!/bin/sh

THREADS=${1:-8}
PROGFILE=${2:-"ppmf-sim.exe"}
SEED=${3:-$RANDOM}

CHECK=`find . -type d | wc -l`
if [ $CHECK -gt 1 ]; then
	echo "ERROR: some directory exist in cwd"
	exit 1
fi

echo "INFO: Running on `hostname` with $THREADS threads using seed = $SEED"

DIR=`dirname $0`

mkdir -p ./input/tmp
mkdir -p ./input/2012-images4sim
mkdir -p ./output

mosaicTxtFilename=mosaic.txt
simCoreParamsFilename=sim-core-params.txt

INITCFG=$DIR/../input/$mosaicTxtFilename
if [ -e ./$mosaicTxtFilename ]
then
	INITCFG=./$mosaicTxtFilename
fi
cp $INITCFG ./input/
IMGFILE=`grep ^image ./input/mosaic.txt | cut -d'=' -f2 | cut -d';' -f1`
cp $DIR/../$IMGFILE ./$IMGFILE

PARAMFILE=$DIR/../input/sim-core-params.txt
if [ -e ./$simCoreParamsFilename ]
then
	PARAMFILE=./$simCoreParamsFilename
fi
cp $PARAMFILE ./input/


# loading site specific variables
. $(dirname $0)/plg-env.cfg

export LD_LIBRARY_PATH
echo "LD_LIBRARY_PATH='${LD_LIBRARY_PATH}'"

echo "BEGIN: `/bin/date`"

$DIR/../bin/init.exe -e $SEED -c $INITCFG

echo "SEGM: `/bin/date`"
echo "EXEC: $PROGFILE"

$DIR/../bin/$PROGFILE $THREADS $SEED $IMGFILE

echo "END: `/bin/date`"

