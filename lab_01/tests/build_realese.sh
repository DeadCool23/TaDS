#!/bin/bash

gcc -c ./../src/*.c -O3 -std=c99 -Wall -Werror -Wpedantic -Wextra -Wvla -Wfloat-equal -Wfloat-conversion
gcc ./*.o -o ./../build/app.exe -lm
