#!/bin/sh

DIR=`dirname $0`

cd $DIR

./make-utils.sh
./make-ppmf.sh

./make-detect.sh
./make-init.sh

cd -

