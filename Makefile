TARGET = libatomic.a

SOURCES :=					\
	atomic_flag.c				\
	atomic_fence.c				\
	atomic_lock.c

OBJECTS := ${SOURCES:.c=.o} atomic_generic.o
ASSEMBS := ${SOURCES:.c=.s}
DEPENDS := ${SOURCES:.c=.dep}
MEMBERS := ${patsubst %.o, ${TARGET}(%.o),${OBJECTS}}

FUNCS =						\
	load					\
	store					\
	exchange				\
	compare_exchange

.INTERMEDIATE : atomic_generic-tmp.o

OBJOPTS := ${shell echo ${FUNCS} | sed 's/\([a-z0-9_][a-z0-9_]*\)/ --defsym=__atomic_\1=atomic_\1_internal /g'}

CFLAGS ?= -O3 -Wall -Wno-shadow

CFLAGS := ${CFLAGS} ${CONFIG}

${TARGET} : ${MEMBERS}

atomic_generic-tmp.o : atomic_generic.c
	${CC} -c ${CFLAGS} -o atomic_generic-tmp.o atomic_generic.c

atomic_generic.o : atomic_generic-tmp.o
	${LD} -r ${OBJOPTS} atomic_generic-tmp.o -o atomic_generic.o


%.s : %.c
	${CC} -S ${CFLAGS} $*.c

%.dep : %.c
	@${CC} -M ${CFLAGS} -o $@ $*.c
	@${CC} -M ${CFLAGS} $*.c | sed 's/[.]o\>/.s/g' >> $@

clean :
	rm -fr ${OBJECTS} ${DEPENDS} ${ASSEMBS} libatomic.a

-include ${DEPENDS}
