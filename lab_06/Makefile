CC = gcc
CFLAGS = -std=gnu99 -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion
DYNLIBS = -lm
INC= -I inc

BUILDDIR = target
TARGET = $(BUILDDIR)/app.exe

SRC = $(wildcard src/*.c)
HEADERS = $(wildcard inc/*.h)

OBJDIR = $(BUILDDIR)/out
OBJ = $(patsubst src/%.c, $(OBJDIR)/%.o, $(SRC))

.PHONY: run
run: $(TARGET)
	./$(TARGET)

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
	$(RM) -r $(BUILDDIR)

$(TARGET): $(OBJDIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(DYNLIBS)

$(OBJDIR): $(BUILDDIR)
	@mkdir -p $@

$(BUILDDIR):
	@mkdir -p $@

$(OBJDIR)/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
