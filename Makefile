ROOT := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
OBJDIR := $(ROOT)/build
LIBOUT := $(ROOT)/arena.so
CC := clang++
CCFLAGS := -I$(ROOT)/include/ \
    -I$(ROOT)/raylib/include/ \
    -Wall \
    -c
LD := clang++
LDFLAGS := -shared
SOURCES := $(shell find $(ROOT)/core/ -type f -name '*.cpp')
OBJECTS := $(patsubst $(ROOT)/core/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

all: build link

build: $(OBJECTS)

link: $(LIBOUT)

$(OBJDIR)/%.o: $(ROOT)/core/%.cpp | $(OBJDIR)
	$(CC) $(CCFLAGS) $< -o $@

$(LIBOUT): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR)
	@rm -f $(LIBOUT)