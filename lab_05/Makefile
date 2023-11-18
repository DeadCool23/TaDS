CC = gcc
CFLAGS = -std=gnu99 -Wall -Werror -Wpedantic -Wextra -Wvla -Wfloat-equal -Wfloat-conversion
DYNLIBS = -lm
INC= -I inc

BUILD_DIR = target
TARGET = $(BUILD_DIR)/app.exe

SRC = $(wildcard src/*.c)
HEADERS = $(wildcard inc/*.h)

OBJDIR = $(BUILD_DIR)/out
OBJ = $(patsubst src/%.c, $(OBJDIR)/%.o, $(SRC))


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
	$(RM) -r $(BUILD_DIR)

$(TARGET): $(BUILD_DIR) $(OBJDIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(DYNLIBS)

$(BUILD_DIR):
	@mkdir -p $@

$(OBJDIR):
	@mkdir -p $@

$(OBJDIR)/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
