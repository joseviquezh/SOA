#!/bin/bash

./creator &
./producer &
sleep 2
./consumer &
sleep 1
#sleep 100
#./finalizer &
