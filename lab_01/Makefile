CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wpedantic -Wextra -Wvla -Wfloat-equal -Wfloat-conversion -lm
TARGET = build/app.exe
DEBUG = -DOUTPUT
FUNC_TESTING = testing.sh
TEST_DIR = tests
SRC = $(wildcard src/*.c)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: build
build: $(TARGET)

.PHONY: func
func: $(TEST_DIR)/$(FUNC_TESTING)
	cd $(TEST_DIR) && bash $(FUNC_TESTING) -v

.PHONY: debug
debug: CFLAGS += $(DEBUG)
debug: $(TARGET)

.PHONY: clean
clean:
	rm ./build/app.exe

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@
