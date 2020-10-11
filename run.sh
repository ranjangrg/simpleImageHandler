#!/usr/bin/bash

MAIN_EXEC_FILE=./test
if test -f "$MAIN_EXEC_FILE"; then
	rm $MAIN_EXEC_FILE;
fi

g++ --std=c++14 test.cpp -g -o test;

if test -f "$MAIN_EXEC_FILE"; then
	$MAIN_EXEC_FILE;
fi

