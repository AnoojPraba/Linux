CC := gcc
CXX := g++
# -pthread is harmless for non-threaded programs and needed to link
# 49_Threads/*.c against pthread. -lm is likewise harmless when unused and
# needed to link 35_TimeAndMath/02_mathFunctions.c against libm.
CFLAGS := -Wall -I. -pthread
CXXFLAGS := -Wall -I.
LDLIBS := -lm
BIN_DIR := ../bin

C_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.c' -print)
CPP_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.cpp' -print)

# 27_MultiFile/mathutils.c, 75_ExternC/mathutils.c, 36_OpaquePointer/handle.c,
# and 73_LinkerAndLoaderMechanics/dataSections.c have no main() - each is a
# helper TU linked into another binary below, not a standalone program - so
# exclude all four from the generic one-file-per-binary rule.
# 74_DynamicLoading/plugin.c is built as a shared library, not linked into
# any standalone binary at all - exclude it too.
# 82_StaticAndSharedLibraries/libgreet.c has no main() either - it becomes
# libgreet.a and libgreet.so, linked into 01_useStatic and 02_useShared
# below - exclude it from the generic one-file-per-binary rule too.
C_SRCS := $(filter-out ./27_MultiFile/mathutils.c ./75_ExternC/mathutils.c \
                       ./36_OpaquePointer/handle.c ./74_DynamicLoading/plugin.c \
                       ./73_LinkerAndLoaderMechanics/dataSections.c \
                       ./82_StaticAndSharedLibraries/libgreet.c,$(C_SRCS))

# 75_ExternC/01_main.cpp is linked against a C-compiled object (see its
# rule below), not built as a plain C++-only binary - exclude it from the
# generic .cpp rule.
CPP_SRCS := $(filter-out ./75_ExternC/01_main.cpp,$(CPP_SRCS))

C_BINS := $(patsubst ./%.c,$(BIN_DIR)/%,$(C_SRCS))
CPP_BINS := $(patsubst ./%.cpp,$(BIN_DIR)/%,$(CPP_SRCS))

.PHONY: all clean

all: $(C_BINS) $(CPP_BINS) $(BIN_DIR)/75_ExternC/01_main $(BIN_DIR)/74_DynamicLoading/01_loader \
     $(BIN_DIR)/36_OpaquePointer/01_main $(BIN_DIR)/73_LinkerAndLoaderMechanics/01_main \
     $(BIN_DIR)/82_StaticAndSharedLibraries/01_useStatic \
     $(BIN_DIR)/82_StaticAndSharedLibraries/02_useShared

$(BIN_DIR)/27_MultiFile/01_main: 27_MultiFile/01_main.c 27_MultiFile/mathutils.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BIN_DIR)/36_OpaquePointer/01_main: 36_OpaquePointer/01_main.c 36_OpaquePointer/handle.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BIN_DIR)/73_LinkerAndLoaderMechanics/01_main: 73_LinkerAndLoaderMechanics/01_main.c \
                                                 73_LinkerAndLoaderMechanics/dataSections.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# 75_ExternC demonstrates a C++ caller linking against a C-compiled TU via
# extern "C" - mathutils.c is compiled as C, 01_main.cpp as C++, and g++
# links the two resulting object files together.
$(BIN_DIR)/75_ExternC/01_main: 75_ExternC/01_main.cpp 75_ExternC/mathutils.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c 75_ExternC/mathutils.c -o $(BIN_DIR)/75_ExternC/mathutils.o
	$(CXX) $(CXXFLAGS) 75_ExternC/01_main.cpp $(BIN_DIR)/75_ExternC/mathutils.o -o $@

# 74_DynamicLoading demonstrates dlopen()/dlsym() loading a shared library
# built separately from the program that loads it at runtime - plugin.c
# becomes libplugin.so (with -fvisibility=hidden, so only symbols marked
# default-visibility in plugin.c are reachable via dlsym), and 01_loader
# links against libdl to call dlopen/dlsym/dlclose.
$(BIN_DIR)/74_DynamicLoading/01_loader: 74_DynamicLoading/01_loader.c 74_DynamicLoading/plugin.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -fvisibility=hidden -shared 74_DynamicLoading/plugin.c \
		-o $(BIN_DIR)/74_DynamicLoading/libplugin.so
	$(CC) $(CFLAGS) 74_DynamicLoading/01_loader.c -o $@ -ldl

# 82_StaticAndSharedLibraries demonstrates linking against the same code
# both as a static archive (.a, copied into the binary at link time) and a
# shared library (.so, resolved by ld.so at load time). rpath is set so the
# shared binary finds libgreet.so next to it without needing
# LD_LIBRARY_PATH set manually.
$(BIN_DIR)/82_StaticAndSharedLibraries/01_useStatic: \
                82_StaticAndSharedLibraries/01_useStatic.c \
                82_StaticAndSharedLibraries/libgreet.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c 82_StaticAndSharedLibraries/libgreet.c \
		-o $(BIN_DIR)/82_StaticAndSharedLibraries/libgreet.o
	ar rcs $(BIN_DIR)/82_StaticAndSharedLibraries/libgreet.a \
		$(BIN_DIR)/82_StaticAndSharedLibraries/libgreet.o
	$(CC) $(CFLAGS) 82_StaticAndSharedLibraries/01_useStatic.c \
		$(BIN_DIR)/82_StaticAndSharedLibraries/libgreet.a -o $@

$(BIN_DIR)/82_StaticAndSharedLibraries/02_useShared: \
                82_StaticAndSharedLibraries/02_useShared.c \
                82_StaticAndSharedLibraries/libgreet.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -shared 82_StaticAndSharedLibraries/libgreet.c \
		-o $(BIN_DIR)/82_StaticAndSharedLibraries/libgreet.so
	$(CC) $(CFLAGS) 82_StaticAndSharedLibraries/02_useShared.c \
		-L$(BIN_DIR)/82_StaticAndSharedLibraries -lgreet \
		-Wl,-rpath,'$$ORIGIN' -o $@

$(BIN_DIR)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

$(BIN_DIR)/%: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)
