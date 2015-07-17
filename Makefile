SOURCES = atomic_flag.c
OBJECTS = ${SOURCES:.c=.o}
ASSEMBS = ${SOURCES:.c=.s}
DEPENDS = ${SOURCES:.c=.dep}

libatomic.a : ${OBJECTS}
	${AR} rc $@ $<

%.s : %.c
	${CC} ${CFLAGS} -S $^
%.dep : %.c
	${CC} ${CFLAGS} -M -o $@ $^

clean :
	rm -r ${OBJECTS} ${DEPENDS} ${ASSEMBS} libatomic.a

include ${DEPENDS}
