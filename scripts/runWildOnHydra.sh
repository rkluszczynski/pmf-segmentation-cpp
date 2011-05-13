#!/bin/sh

CHECK=`find . -type d | wc -l`
if [ $CHECK -gt 1 ]; then
	echo "ERROR: some directory exist in cwd"
	exit 1
fi

THREADS=${1:-4}
HOW_MANY_JOBS=3

CWD="`pwd`"
JOBSLOG=$CWD/qsub.log

hostname > $JOBSLOG
pwd >> $JOBSLOG
date >> $JOBSLOG
for i in `seq 1 $HOW_MANY_JOBS`
do
	echo "Number $i"
	DIR="$CWD/sim-num-$i"
	SEED=""
	mkdir $DIR
	cp `dirname $0`/runOnHydra.pbs $DIR/runOnHydra-no$i.pbs
	sed -i "s!DIR!${DIR}!" $DIR/runOnHydra-no$i.pbs
	sed -i "s!THREADS!${THREADS}!" $DIR/runOnHydra-no$i.pbs
	sed -i "s!SEED!${SEED}!" $DIR/runOnHydra-no$i.pbs
	cd $DIR
	pwd >> $JOBSLOG
	qsub $DIR/runOnHydra-no$i.pbs >> $JOBSLOG
	cd -
done

date >> $JOBSLOG
