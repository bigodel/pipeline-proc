include ./Makefile.config

PROJECT := mips
SRCS    := $(wildcard *.cpp)
OBJS    := $(SRCS:.cpp=.o)

include ./Makefile.rules
