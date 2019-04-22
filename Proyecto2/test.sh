#!/bin/bash

if [ $# -gt 0 -o $# != 2 ]; then
    echo "Not enough arguments"
    break
fi

./creator --buffer /sh_mem&
./finalizer --time 5 --buffer /sh_mem&

PROD_NUM=$1
CONS_NUM=$2

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
