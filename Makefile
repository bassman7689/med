CC=gcc
CFLAGS=
LDFLAGS=-lncurses -macosx_version_min 10.12 -lc
BUILDDIR=./build
SOURCEDIR=./src
OBJS=${BUILDDIR}/gap_buffer.o ${BUILDDIR}/editor.o ${BUILDDIR}/main.o
SRCS=${SOURCEDIR}/gap_buffer.c ${SOURCEDIR}/editor.c ${SOURCEDIR}/main.c

all: ${BUILDDIR} ${OBJS}
	ld -o ${BUILDDIR}/med ${OBJS} ${LDFLAGS}

${BUILDDIR}/%.o: ${SOURCEDIR}/%.c
	${CC} ${CFLAGS} -c $< -o $@

${BUILDDIR}:
	mkdir -p build

.PHONY: clean

clean:
	rm -rf build
