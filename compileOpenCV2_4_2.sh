#!/bin/sh
export LD_LIBRARY_PATH=/usr/lib
echo `pkg-config --cflags opencv`
echo `pkg-config --libs opencv`
g++ `pkg-config --cflags opencv` -g -o facedetect facedetect.cpp digitalCloset_Motorcontrol.c  `pkg-config --libs opencv`
