#!/bin/sh
#
# printing :  filename; num.of threads; (pmr; itersnum;) per thread; winning thread; time in sec.
#

function analyzeSimulation
{
	STDOUT=${1-.}/stdout
	STDERR=${1-.}/stderr
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
		if [ "${THsNUM}" -eq "1" ]
		then
			ITERsNUM=`grep "^\[ ITER \] _singiel_" $STDOUT | tail -1 | cut -d':' -f2`
		else		
			ITERsNUM=`grep "^\[ ITER \] _th${i}of${THsNUM}_" $STDOUT | tail -1 | cut -d':' -f2`
		fi
		#echo Thread-$i : $ITERsNUM
		k=$(($i + 2))
		ThPMR=$(echo ${THsPMRs} | cut -d'=' -f${k} | cut -d'(' -f1)
		RECORD="${RECORD} ${ThPMR}; ${ITERsNUM};"
	done 

	StoppingThread=$(grep ended $STDOUT | cut -d'_' -f3)
	SegFault=$(grep "Segmentation fault" ${STDERR})
	JobKilled=$(grep "PBS: job killed" ${STDERR})
	if [ "${SegFault}" != "" ]
	then
		RECORD="${RECORD} -1;"
	elif [ "${JobKilled}" != "" ]
	then
		RECORD="${RECORD} -2;"
	else
		RECORD="${RECORD} ${StoppingThread};"
	fi

	SIMSTART=$(grep "^BEGIN: " ${STDOUT} | cut -b 8-)
	SIMSTOP=$(grep "^END: " ${STDOUT} | cut -b 6-)
	SIMTIME=$(( $(date +%s -d "${SIMSTOP}") - $(date +%s -d "${SIMSTART}") ))
	RECORD="${RECORD} ${SIMTIME};"

	echo ${RECORD}
	#tail ${STDERR} | grep PBS
}

for entry in `ls`
do
	if [ -d $entry ]
	then
		analyzeSimulation $entry
	fi
done

