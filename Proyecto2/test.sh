#!/bin/bash

./creator &
./producer &
./consumer &
sleep 1
./finalizer &
