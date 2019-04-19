#!/bin/bash

./creator &
./producer &
sleep 1
./consumer &
sleep 1
./producer &
sleep 1
./consumer &
#sleep 100
#./finalizer &
