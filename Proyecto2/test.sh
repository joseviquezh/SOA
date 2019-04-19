#!/bin/bash

./creator &
./producer &
sleep 2
./consumer &
sleep 1
./finalizer &
