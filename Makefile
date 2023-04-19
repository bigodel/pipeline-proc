# -*- mode: makefile-gmake; -*-

include ./Makefile.config

# project settings
PROJECT := mips
SRCS    := $(wildcard *.cpp)
OBJS    := $(SRCS:.cpp=.o)

# default compiler flags
FLAGS_COMMON = -g -Wall
FLAGS_STRICT = -pedantic -Wno-long-long
FLAGS_WERROR = -Werror

# default linker flags
LDFLAG_RPATH ?= -Wl,-rpath=

# variable that points to SystemC installation path
SYSTEMC_HOME ?= $SYSTEMC_HOME

# select the target architecture
TARGET_ARCH ?= linux

# select the architecture suffix, if necessary
ARCH_SUFFIX ?= -$(TARGET_ARCH)

# explicit location of the systemc headers
SYSTEMC_INC_DIR ?= $(SYSTEMC_HOME)/include

# explicit location if the systenc library
SYSTEMC_LIB_DIR ?= $(SYSTEMC_HOME)/lib$(ARCH_SUFFIX)

# combined flags
SYSTEMC_CXXFLAGS = $(FLAGS_COMMON) $(FLAGS_STRICT) $(FLAGS_WERROR)

SYSTEMC_LDFLAGS  ?= -L$(SYSTEMC_LIB_DIR) $(LDFLAG_RPATH)$(SYSTEMC_LIB_DIR)

SYSTEMC_LIBS     ?= -lsystemc -lm

# include directories
INCDIR   += -I. -I.. -Isrc/ -I$(SYSTEMC_INC_DIR)

# library directories
LIBDIR   += -L. -L..

# compilation flags
CXXFLAGS  += $(CFLAGS) $(SYSTEMC_CXXFLAGS) $(INCDIR) $(SYSTEMC_DEFINES)
LDFLAGS   += $(CFLAGS) $(SYSTEMC_CXXFLAGS) $(LIBDIR) $(SYSTEMC_LDFLAGS)
LIBS      += $(SYSTEMC_LIBS) $(EXTRA_LIBS)

## ***************************************************************************
## build rules

.SUFFIXES: .cpp .o

all: build

run: all
	./$(PROJECT) $(ARGS)

build: $(PROJECT)

$(PROJECT): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) 2>&1

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(PROJECT)
