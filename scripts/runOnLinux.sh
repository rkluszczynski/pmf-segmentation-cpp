#!/bin/sh

THREADS=${1:-8}
SEED=${2:-$RANDOM}

CHECK=`find . -type d | wc -l`
if [ $CHECK -gt 1 ]; then
	echo "ERROR: some directory exist in cwd"
	exit 1
fi

echo "INFO: Running on `hostname` with $THREADS threads using seed = $SEED"

DIR=`dirname $0`
INITCFG=$DIR/../input/mosaic.txt

mkdir -p ./input/tmp
mkdir -p ./output

cp $INITCFG ./input/
IMGFILE=`grep ^image ./input/mosaic.txt | cut -d'=' -f2 | cut -d';' -f1`
cp $DIR/../$IMGFILE ./$IMGFILE


export LD_LIBRARY_PATH=/vosoft/unicore/pmf/wx-2.8.12/lib
echo "BEGIN: `/bin/date`"

$DIR/../bin/init.exe -e $SEED -c $INITCFG

echo "SEGM: `/bin/date`"

$DIR/../bin/ppmf-sim.exe $THREADS $SEED $IMGFILE

echo "END: `/bin/date`"
