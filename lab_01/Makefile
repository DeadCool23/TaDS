CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wpedantic -Wextra -Wvla -Wfloat-equal -Wfloat-conversion -lm
TARGET = build/app.exe
DEBUG = -DOUTPUT
FUNC_TESTING = testing.sh
TEST_DIR = tests
SRC = $(wildcard src/*.c)

.PHONY: run debug build clean

run: $(TARGET)
	./$(TARGET)

build: $(TARGET)

func:
	cd $(TEST_DIR) && bash $(FUNC_TESTING) -v

debug:
	$(CC) $(CFLAGS) $(DEBUG) -g $(SRC) -o $(TARGET)

clean: $(TARGET)
	rm ./$(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@
