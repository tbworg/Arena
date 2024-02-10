ROOT := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
OBJOUT := $(ROOT)/build/arena.o
LIBOUT := $(ROOT)/arena.so
CC := clang++
CCFLAGS := -I$(ROOT)/include/ \
	-Wall \
	-c
LD := clang 
LDFLAGS := -shared
SOURCES := $(shell find $(ROOT)/core/ -type f -name '*.cpp')

all: build link

build: $(OBJOUT)
link: $(LIBOUT)

$(OBJOUT): $(SOURCES)
	$(CC) $(CCFLAGS) $(SOURCES) -o $(OBJOUT)

$(LIBOUT): $(OBJOUT)
	$(LD) $(LDFLAGS) $(OBJOUT) -o $(LIBOUT)