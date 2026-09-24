# Auto-parallelize: `make` alone runs with one job per CPU core, same as
# passing -j$(nproc) by hand. An explicit `-j<N>` on the command line still
# overrides this.
MAKEFLAGS += -j$(shell nproc)

CXX := g++
CXXFLAGS := -std=c++17 -Wall -I. -pthread
BIN_DIR := bin

CPP_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.cpp' -print)
CPP_BINS := $(patsubst ./%.cpp,$(BIN_DIR)/%,$(CPP_SRCS))

.PHONY: all clean

all: $(CPP_BINS)

$(BIN_DIR)/%: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)
