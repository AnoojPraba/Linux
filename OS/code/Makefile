# Auto-parallelize: `make` alone runs with one job per CPU core, same as
# passing -j$(nproc) by hand. An explicit `-j<N>` on the command line still
# overrides this.
MAKEFLAGS += -j$(shell nproc)

CC := gcc
CXX := g++
# -pthread is needed to link 08_Threads/*.c and other synchronization demos
# against pthread. -lm is harmless when unused.
CFLAGS := -Wall -I. -pthread
CXXFLAGS := -Wall -I.
LDLIBS := -lm
BIN_DIR := ../bin

C_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.c' -print)
CPP_SRCS := $(shell find . -path ./$(BIN_DIR) -prune -o -name '*.cpp' -print)

# 59_LinkerAndLoaderMechanics/dataSections.c has no main() - it is a helper
# TU linked into 01_main below, not a standalone program - exclude it from
# the generic one-file-per-binary rule.
# 60_DynamicLoading/plugin.c is built as a shared library, not linked into
# any standalone binary at all - exclude it too.
# 61_StaticAndSharedLibraries/libgreet.c has no main() either - it becomes
# libgreet.a and libgreet.so, linked into 01_useStatic and 02_useShared
# below - exclude it from the generic one-file-per-binary rule too.
C_SRCS := $(filter-out ./59_LinkerAndLoaderMechanics/dataSections.c \
                       ./60_DynamicLoading/plugin.c \
                       ./61_StaticAndSharedLibraries/libgreet.c,$(C_SRCS))

C_BINS := $(patsubst ./%.c,$(BIN_DIR)/%,$(C_SRCS))
CPP_BINS := $(patsubst ./%.cpp,$(BIN_DIR)/%,$(CPP_SRCS))

.PHONY: all clean

all: $(C_BINS) $(CPP_BINS) $(BIN_DIR)/60_DynamicLoading/01_loader \
     $(BIN_DIR)/59_LinkerAndLoaderMechanics/01_main \
     $(BIN_DIR)/61_StaticAndSharedLibraries/01_useStatic \
     $(BIN_DIR)/61_StaticAndSharedLibraries/02_useShared

$(BIN_DIR)/59_LinkerAndLoaderMechanics/01_main: 59_LinkerAndLoaderMechanics/01_main.c \
                                                 59_LinkerAndLoaderMechanics/dataSections.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# 60_DynamicLoading demonstrates dlopen()/dlsym() loading a shared library
# built separately from the program that loads it at runtime - plugin.c
# becomes libplugin.so (with -fvisibility=hidden, so only symbols marked
# default-visibility in plugin.c are reachable via dlsym), and 01_loader
# links against libdl to call dlopen/dlsym/dlclose.
$(BIN_DIR)/60_DynamicLoading/01_loader: 60_DynamicLoading/01_loader.c 60_DynamicLoading/plugin.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -fvisibility=hidden -shared 60_DynamicLoading/plugin.c \
		-o $(BIN_DIR)/60_DynamicLoading/libplugin.so
	$(CC) $(CFLAGS) 60_DynamicLoading/01_loader.c -o $@ -ldl

# 61_StaticAndSharedLibraries demonstrates linking against the same code
# both as a static archive (.a, copied into the binary at link time) and a
# shared library (.so, resolved by ld.so at load time). rpath is set so the
# shared binary finds libgreet.so next to it without needing
# LD_LIBRARY_PATH set manually.
$(BIN_DIR)/61_StaticAndSharedLibraries/01_useStatic: \
                61_StaticAndSharedLibraries/01_useStatic.c \
                61_StaticAndSharedLibraries/libgreet.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c 61_StaticAndSharedLibraries/libgreet.c \
		-o $(BIN_DIR)/61_StaticAndSharedLibraries/libgreet.o
	ar rcs $(BIN_DIR)/61_StaticAndSharedLibraries/libgreet.a \
		$(BIN_DIR)/61_StaticAndSharedLibraries/libgreet.o
	$(CC) $(CFLAGS) 61_StaticAndSharedLibraries/01_useStatic.c \
		$(BIN_DIR)/61_StaticAndSharedLibraries/libgreet.a -o $@

$(BIN_DIR)/61_StaticAndSharedLibraries/02_useShared: \
                61_StaticAndSharedLibraries/02_useShared.c \
                61_StaticAndSharedLibraries/libgreet.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -shared 61_StaticAndSharedLibraries/libgreet.c \
		-o $(BIN_DIR)/61_StaticAndSharedLibraries/libgreet.so
	$(CC) $(CFLAGS) 61_StaticAndSharedLibraries/02_useShared.c \
		-L$(BIN_DIR)/61_StaticAndSharedLibraries -lgreet \
		-Wl,-rpath,'$$ORIGIN' -o $@

# 47_NamedPipesAndPosixQueues/02_posixMessageQueue.c uses mq_open()/mq_send()/
# mq_receive(), which on this platform require linking against librt (glibc
# ships them there, not in libc proper) - verified by a link failure without
# -lrt, so it's added just for this one binary rather than globally.
$(BIN_DIR)/47_NamedPipesAndPosixQueues/02_posixMessageQueue: \
                47_NamedPipesAndPosixQueues/02_posixMessageQueue.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS) -lrt

$(BIN_DIR)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

$(BIN_DIR)/%: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)
