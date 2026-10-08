# Auto-parallelize: `make` alone runs with one job per CPU core, same as
# passing -j$(nproc) by hand. An explicit `-j<N>` on the command line still
# overrides this.
MAKEFLAGS += -j$(shell nproc)

CXX := g++
CXXFLAGS := -std=c++17 -Wall -I. -pthread
BIN_DIR := bin

# 33_Cpp20Coroutines and 34_SpanStringViewAndPmr use real C++20 features
# (<coroutine>, <span>); build just those folders with -std=c++20 (needs
# GCC >= 10) while everything else stays on C++17.
CXX20FLAGS := -std=c++20 -Wall -I. -pthread
$(BIN_DIR)/33_%: CXXFLAGS := $(CXX20FLAGS)
$(BIN_DIR)/34_%: CXXFLAGS := $(CXX20FLAGS)

CPP_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.cpp' -print)
CPP_BINS := $(patsubst ./%.cpp,$(BIN_DIR)/%,$(CPP_SRCS))

.PHONY: all clean

all: $(CPP_BINS)

$(BIN_DIR)/%: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)
