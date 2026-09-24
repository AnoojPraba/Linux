# Auto-parallelize: `make` alone runs with one job per CPU core, same as
# passing -j$(nproc) by hand. An explicit `-j<N>` on the command line still
# overrides this.
MAKEFLAGS += -j$(shell nproc)

CC := gcc
CXX := g++
# -pthread is harmless for non-threaded programs even though no folder here
# needs it anymore (threading topics moved to ../../OS/code). -lm is likewise
# harmless when unused and needed to link 47_TimeAndMath/02_mathFunctions.c
# against libm.
CFLAGS := -Wall -I. -pthread
CXXFLAGS := -Wall -I.
LDLIBS := -lm
BIN_DIR := ../bin

C_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.c' -print)
CPP_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.cpp' -print)

# 30_MultiFile/mathutils.c and 59_ExternC/mathutils.c have no main() - each is
# a helper TU linked into another binary below, not a standalone program - so
# exclude both from the generic one-file-per-binary rule.
# 49_OpaquePointer/handle.c has no main() either, for the same reason.
# 66_CMakeIntroduction/01_hello.c and 02_helper.c are two TUs of one program
# (see the explicit rule below) and also have their own CMakeLists.txt for
# building via CMake directly - exclude both from the generic rule.
C_SRCS := $(filter-out ./30_MultiFile/mathutils.c ./59_ExternC/mathutils.c \
                       ./49_OpaquePointer/handle.c \
                       ./66_CMakeIntroduction/01_hello.c \
                       ./66_CMakeIntroduction/02_helper.c,$(C_SRCS))

# 59_ExternC/01_main.cpp is linked against a C-compiled object (see its
# rule below), not built as a plain C++-only binary - exclude it from the
# generic .cpp rule.
CPP_SRCS := $(filter-out ./59_ExternC/01_main.cpp,$(CPP_SRCS))

C_BINS := $(patsubst ./%.c,$(BIN_DIR)/%,$(C_SRCS))
CPP_BINS := $(patsubst ./%.cpp,$(BIN_DIR)/%,$(CPP_SRCS))

.PHONY: all clean

all: $(C_BINS) $(CPP_BINS) $(BIN_DIR)/59_ExternC/01_main \
     $(BIN_DIR)/49_OpaquePointer/01_main $(BIN_DIR)/66_CMakeIntroduction/01_hello

$(BIN_DIR)/30_MultiFile/01_main: 30_MultiFile/01_main.c 30_MultiFile/mathutils.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BIN_DIR)/49_OpaquePointer/01_main: 49_OpaquePointer/01_main.c 49_OpaquePointer/handle.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BIN_DIR)/66_CMakeIntroduction/01_hello: 66_CMakeIntroduction/01_hello.c \
                                          66_CMakeIntroduction/02_helper.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# 59_ExternC demonstrates a C++ caller linking against a C-compiled TU via
# extern "C" - mathutils.c is compiled as C, 01_main.cpp as C++, and g++
# links the two resulting object files together.
$(BIN_DIR)/59_ExternC/01_main: 59_ExternC/01_main.cpp 59_ExternC/mathutils.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c 59_ExternC/mathutils.c -o $(BIN_DIR)/59_ExternC/mathutils.o
	$(CXX) $(CXXFLAGS) 59_ExternC/01_main.cpp $(BIN_DIR)/59_ExternC/mathutils.o -o $@

$(BIN_DIR)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

$(BIN_DIR)/%: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)
