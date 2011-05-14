#!/bin/sh
#
# printing :  filename; num.of threads; (pmr; itersnum;) per thread; winning thread; time in sec.
#

function analyzeSimulation
{
	STDOUT=${1-.}/stdout
	if [ ! -e ${STDOUT} ]
	then
		echo file $STDOUT not existing ... ignoring
		return
	fi
	RECORD="${STDOUT};"

	#ITERS=`grep "[ ITER ]" | cut -d':' -f1 | uniq`
	THsNUM=`head -1 $STDOUT | cut -d' ' -f6`
	RECORD="${RECORD} ${THsNUM};"

	THsPMRs=$(grep "pmr during sync" $STDOUT | tail -1)
	#echo $THsPMRs
	LASTTH=$((${THsNUM} - 1))
	for i in `seq 0 ${LASTTH}`
	do
		ITERsNUM=`grep "^\[ ITER \] _th${i}of${THsNUM}_" $STDOUT | tail -1 | cut -d':' -f2`
		#echo Thread-$i : $ITERsNUM
		k=$(($i + 2))
		ThPMR=$(echo ${THsPMRs} | cut -d'=' -f${k} | cut -d'(' -f1)
		RECORD="${RECORD} ${ThPMR}; ${ITERsNUM};"
	done 

	StoppingThread=$(grep ended $STDOUT | cut -d'_' -f3)
	RECORD="${RECORD} ${StoppingThread};"

	SIMSTART=$(grep "^BEGIN: " ${STDOUT} | cut -b 8-)
	SIMSTOP=$(grep "^END: " ${STDOUT} | cut -b 6-)
	SIMTIME=$(( $(date +%s -d "${SIMSTOP}") - $(date +%s -d "${SIMSTART}") ))
	RECORD="${RECORD} ${SIMTIME};"

	echo ${RECORD}
}

for entry in `ls`
do
	if [ -d $entry ]
	then
		analyzeSimulation $entry
	fi
done

