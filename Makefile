CC=gcc
CFLAGS=-g
LDFLAGS=-lncurses
BUILDDIR=./build
SRCDIR=./src
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))
APPNAME=med

all: $(BUILDDIR) $(APPNAME)

$(APPNAME): $(OBJS)
	$(CC) $^ -o $(BUILDDIR)/$(APPNAME) $(CFLAGS) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${BUILDDIR}:
	mkdir -p build

.PHONY: run

run: $(APPNAME)
	$(BUILDDIR)/$(APPNAME)


.PHONY: clean

clean:
	rm -rf build
