#!/bin/bash

if [ $# -lt 0 -o $# != 3 ]; then
    echo "Not enough arguments"
    echo "Usage: ./test.sh <PROD_NUM> <CONS_NUM> <FIN_TIMER>"
    exit 1
fi

PROD_NUM=$1
CONS_NUM=$2
FIN_TIMER=$3

./creator --buffer /sh_mem&
sleep 1
./finalizer --time $FIN_TIMER --buffer /sh_mem&

while [ $PROD_NUM -gt 0 ];
do
./producer --buffer /sh_mem&
let PROD_NUM=$PROD_NUM-1

done

while [ $CONS_NUM -gt 0 ];
do
./consumer --buffer /sh_mem&
let CONS_NUM=$CONS_NUM-1

done
