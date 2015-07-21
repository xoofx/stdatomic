TARGET = libatomic.a

SOURCES :=					\
	atomic_flag.c				\
	atomic_fence.c				\
	atomic_lock.c

OBJECTS := ${SOURCES:.c=.o} atomic_generic.o 	atomic_gcc_sync.o
ASSEMBS := ${SOURCES:.c=.s}
DEPENDS := ${OBJECTS:.o=.dep}
MEMBERS := ${patsubst %.o, ${TARGET}(%.o),${OBJECTS}}

EFUNCS =					\
	load					\
	store					\
	exchange				\
	compare_exchange

RFUNCS =					\
	load_1					\
	store_1					\
	exchange_1				\
	compare_exchange_1                      \
	load_2					\
	store_2					\
	exchange_2				\
	compare_exchange_2                      \
	load_4					\
	store_4					\
	exchange_4				\
	compare_exchange_4                      \
	load_8					\
	store_8					\
	exchange_8				\
	compare_exchange_8                      \
	load_16					\
	store_16				\
	exchange_16				\
	compare_exchange_16

.INTERMEDIATE : atomic_generic-tmp.o atomic_gcc_sync-tmp.o

LDOPTS := ${shell echo ${EFUNCS} | sed 's/\([a-z0-9_][a-z0-9_]*\)/ --defsym=__atomic_\1=atomic_\1_internal /g'}

OBJOPTS := ${shell echo ${RFUNCS} | sed 's/\([a-z0-9_][a-z0-9_]*\)/ --redefine-sym=atomic_\1_internal=__atomic_\1 /g'}

COPTS ?= -O3 -march=native

CFLAGS ?= -Wall -Wno-shadow -isystem `pwd` ${COPTS}

CFLAGS := ${CFLAGS} ${CONFIG}

target : libatomic.a

ifeq (${MAKELEVEL},0)
# compile all objects in parallel mode and then recurse
libatomic.a : ${OBJECTS}
	${MAKE} libatomic.a
else
# now, addition of the archive members should be done sequentially
.NOTPARALLEL :
libatomic.a : ${MEMBERS}
endif

atomic_generic-tmp.o : atomic_generic.c
	${CC} -c ${CFLAGS} -o atomic_generic-tmp.o atomic_generic.c

atomic_generic.o : atomic_generic-tmp.o
	${LD} -r ${LDOPTS} atomic_generic-tmp.o -o atomic_generic.o

atomic_gcc_sync-tmp.o : atomic_gcc_sync.c
	${CC} -c ${CFLAGS} -o atomic_gcc_sync-tmp.o atomic_gcc_sync.c

atomic_gcc_sync.o : atomic_gcc_sync-tmp.o
	 objcopy -v ${OBJOPTS} atomic_gcc_sync-tmp.o atomic_gcc_sync.o


%.s : %.c
	${CC} -S ${CFLAGS} $*.c

%.dep : %.c
	@${CC} -M ${CFLAGS} -o $@ $*.c
	@${CC} -M ${CFLAGS} $*.c | sed 's/[.]o\>/.s/g' >> $@

clean :
	rm -fr ${OBJECTS} ${DEPENDS} ${ASSEMBS} libatomic.a

-include ${DEPENDS}
