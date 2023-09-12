#!/bin/bash

gcc -c ./../src/*.c -g -O0 -std=c99 -Wall -Werror -Wpedantic -Wextra -Wvla -Wfloat-equal -Wfloat-conversion
gcc ./*.o -o ./../build/app.exe -lm