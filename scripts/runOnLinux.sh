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
INITCFG=$DIR/../input/mosaic.txt

mkdir -p ./input/tmp
mkdir -p ./output

cp $INITCFG ./input/
IMGFILE=`grep ^image ./input/mosaic.txt | cut -d'=' -f2 | cut -d';' -f1`
cp $DIR/../$IMGFILE ./$IMGFILE

if [ -e /vosoft/unicore/pmf/wx-2.8.12/lib ]
then
	export LD_LIBRARY_PATH=/vosoft/unicore/pmf/wx-2.8.12/lib
elif [ -e  /people/plgkluszczynski/pmf/wx-2.8.12/lib ]
then
	export LD_LIBRARY_PATH=/people/plgkluszczynski/pmf/wx-2.8.12/lib
elif [ -e  /home/plgrid/plgkluszczynski/reef/pmf/wx-2.8.12/lib ]
then
	export LD_LIBRARY_PATH=/home/plgrid/plgkluszczynski/reef/pmf/wx-2.8.12/lib
else
	echo "[ ERROR ]: no wxWidgets library!"
	exit 1
fi
echo "LD_LIBRARY_PATH='${LD_LIBRARY_PATH}'"
echo "BEGIN: `/bin/date`"

$DIR/../bin/init.exe -e $SEED -c $INITCFG

echo "SEGM: `/bin/date`"
echo "EXEC: $PROGFILE"

$DIR/../bin/$PROGFILE $THREADS $SEED $IMGFILE

echo "END: `/bin/date`"
