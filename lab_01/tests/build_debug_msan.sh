#!/bin/bash

clang -c ./../src/*.c -O0 -g3 -fsanitize=memory -fPIE -fno-omit-frame-pointer -std=c99 -Wall -Werror -Wextra -Wpedantic
clang -o ./../build/app.exe ./*.o -fsanitize=memory -lm