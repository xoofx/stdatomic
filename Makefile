TARGET = libatomic.a

SOURCES =					\
	atomic_flag.c				\
	atomic_fence.c				\
	atomic_lock.c

OBJECTS = ${SOURCES:.c=.o}
ASSEMBS = ${SOURCES:.c=.s}
DEPENDS = ${SOURCES:.c=.dep}
MEMBERS = ${patsubst %.o, ${TARGET}(%.o),${OBJECTS}}

CFLAGS ?= -O3 -Wall

CFLAGS := ${CFLAGS} ${CONFIG}

${TARGET} : ${MEMBERS}

%.s : %.c
	${CC} -S ${CFLAGS} $*.c
%.dep : %.c
	@${CC} -M ${CFLAGS} -o $@ $*.c
	@${CC} -M ${CFLAGS} $*.c | sed 's/[.]o\>/.s/g' >> $@

clean :
	rm -fr ${OBJECTS} ${DEPENDS} ${ASSEMBS} libatomic.a

-include ${DEPENDS}
