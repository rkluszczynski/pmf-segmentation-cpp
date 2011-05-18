#!/bin/sh
#
PROGFILE=${1:-"../bin/ppmf-sim.exe"}
DIR=`dirname $0`

cd $DIR

./make-utils.sh
./make-ppmf.sh ${PROGFILE}

./make-detect.sh
./make-init.sh

cd -

