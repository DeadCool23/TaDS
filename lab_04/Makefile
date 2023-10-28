CC = gcc
CFLAGS = -std=gnu99 -Wall -Werror -Wpedantic -Wextra -Wvla -Wfloat-equal -Wfloat-conversion
DYNLIBS = -lm
INC= -I inc

FUNC_TESTING = testing.sh
TEST_DIR = func_test
SRC = $(wildcard src/*.c)
HEADERS = $(wildcard inc/*.h)

OBJDIR = out
OBJ = $(patsubst src/%.c, $(OBJDIR)/%.o, $(SRC))

BUILD_DIR = build/
TARGET = $(BUILD_DIR)app.exe
UNITARGET = $(BUILD)unit_tests.exe

.PHONY: build
build: $(TARGET)

.PHONY: release
release: CFLAGS += -O3
release: $(TARGET)

.PHONY: debug
debug: CFLAGS += -g
debug: CFLAGS += -O0
debug: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(BUILD)*.exe -r $(OBJDIR) $(BUILD_DIR)

$(TARGET): $(BUILD_DIR) $(OBJDIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(DYNLIBS)

$(BUILD_DIR):
	@mkdir -p build

$(OBJDIR):
	@mkdir -p out

$(OBJDIR)/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
