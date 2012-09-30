#!/bin/sh
#
#	usage : $0 how_many_jobs how_many_threads
#
if [ $# -lt 2  -o  $# -gt 3 ]
then
	echo "[ USAGE ] : `basename $0` <how_many_jobs> <how_many_threads_each_job> [<program_file_basename>]"
	exit 1
fi

SAVECHECK=`find . -type d | wc -l`
if [ ${SAVECHECK} -gt 1 ]; then
	echo "[ ERROR ] :{`basename $0`}: some directory exist in cwd"
	exit 1
fi

HOW_MANY_JOBS=${1:-10}
THREADS=${2:-4}
PROGFILE=$(basename ${3:-"ppmf-sim.exe"})
QSUB_TEMPLATE=`dirname $0`/runOnHydra.pbs

CWD="`pwd`"
JOBSLOG=${CWD}/qsub-${HOW_MANY_JOBS}x${THREADS}.log

hostname > ${JOBSLOG}
pwd >> ${JOBSLOG}
date >> ${JOBSLOG}
_QSUB_STARTTIME=$(/bin/date +%s)
for i in `seq 1 ${HOW_MANY_JOBS}`
do
	echo "Number ${i}"
	DIR="$CWD/sim-num-${i}"
#	SEED=$(($i * 20 + 100))
	SEED=""
	mkdir ${DIR}
	QSUBFILE=${DIR}/runOnHydra-no${i}.pbs
	cp ${QSUB_TEMPLATE} ${QSUBFILE}
	sed -i "s!DIR!${DIR}!" ${QSUBFILE}
	sed -i "s!THREADS!${THREADS}!" ${QSUBFILE}
	sed -i "s!SEED!${SEED}!" ${QSUBFILE}
	sed -i "s!PROGFILE!${PROGFILE}!" ${QSUBFILE}
	cd ${DIR}
	pwd >> ${JOBSLOG}
	qsub ${QSUBFILE} >> ${JOBSLOG}
	cd -
done
_QSUB_ENDTIME=$(/bin/date +%s)
date >> ${JOBSLOG}
echo "Submission time of ${HOW_MANY_JOBS} tasks = $(expr ${_QSUB_ENDTIME} - ${_QSUB_STARTTIME}) sec." | tee -a ${JOBSLOG}

