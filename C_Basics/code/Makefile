CC := gcc
CXX := g++
# -pthread is harmless for non-threaded programs and needed to link
# 19_Threads/*.c against pthread.
CFLAGS := -Wall -I. -pthread
CXXFLAGS := -Wall -I.
BIN_DIR := bin

C_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.c' -print)
CPP_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.cpp' -print)

# 14_MultiFile/mathutils.c has no main() - it is a helper TU linked into
# 01_main's binary below, not a standalone program - so exclude it from the
# generic one-file-per-binary rule.
C_SRCS := $(filter-out ./14_MultiFile/mathutils.c,$(C_SRCS))

C_BINS := $(patsubst ./%.c,$(BIN_DIR)/%,$(C_SRCS))
CPP_BINS := $(patsubst ./%.cpp,$(BIN_DIR)/%,$(CPP_SRCS))

.PHONY: all clean

all: $(C_BINS) $(CPP_BINS)

$(BIN_DIR)/14_MultiFile/01_main: 14_MultiFile/01_main.c 14_MultiFile/mathutils.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/%: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)
