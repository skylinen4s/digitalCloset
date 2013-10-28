#!/bin/sh
export LD_LIBRARY_PATH=/usr/local/lib
echo `pkg-config --cflags opencv`
echo `pkg-config --libs opencv`
g++ `pkg-config --cflags opencv` -g -o $2 $1 `pkg-config --libs opencv`
