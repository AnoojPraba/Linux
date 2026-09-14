CC := gcc
CXX := g++
CFLAGS := -Wall -I.
CXXFLAGS := -Wall -I.
BIN_DIR := bin

C_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.c' -print)
CPP_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.cpp' -print)

C_BINS := $(patsubst ./%.c,$(BIN_DIR)/%,$(C_SRCS))
CPP_BINS := $(patsubst ./%.cpp,$(BIN_DIR)/%,$(CPP_SRCS))

.PHONY: all clean

all: $(C_BINS) $(CPP_BINS)

$(BIN_DIR)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/%: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)
