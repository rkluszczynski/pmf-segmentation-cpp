#!/bin/sh
#
#	usage : $0 how_many_jobs how_many_threads
#
SAVECHECK=`find . -type d | wc -l`
if [ ${SAVECHECK} -gt 1 ]; then
	echo "ERROR: some directory exist in cwd"
	exit 1
fi

HOW_MANY_JOBS=${1:-10}
THREADS=${2:-4}

QSUB_TEMPLATE=`dirname $0`/runOnHydra.pbs

CWD="`pwd`"
JOBSLOG=${CWD}/qsub-${HOW_MANY_JOBS}x${THREADS}.log

hostname > ${JOBSLOG}
pwd >> ${JOBSLOG}
date >> ${JOBSLOG}
for i in `seq 1 ${HOW_MANY_JOBS}`
do
	echo "Number ${i}"
	DIR="$CWD/sim-num-${i}"
	SEED=$(($i * 20 + 100))
#	SEED=""
	mkdir ${DIR}
	QSUBFILE=${DIR}/runOnHydra-no${i}.pbs
	cp ${QSUB_TEMPLATE} ${QSUBFILE}
	sed -i "s!DIR!${DIR}!" ${QSUBFILE}
	sed -i "s!THREADS!${THREADS}!" ${QSUBFILE}
	sed -i "s!SEED!${SEED}!" ${QSUBFILE}
	cd ${DIR}
	pwd >> ${JOBSLOG}
	qsub ${QSUBFILE} >> ${JOBSLOG}
	cd -
done

date >> ${JOBSLOG}
