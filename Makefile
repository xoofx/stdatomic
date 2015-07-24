TARGET = libatomic.a

MUSL = ${HOME}/build/musl

SOURCES :=					\
	atomic_flag.c				\
	atomic_fence.c

GENERICS :=					\
	atomic_generic.c			\
	atomic_generic_1.c			\
	atomic_generic_2.c			\
	atomic_generic_4.c			\
	atomic_generic_8.c			\
	atomic_generic_16.c

OBJECTS := ${SOURCES:.c=.o} ${GENERICS:.c=.o} lock-repl.o
ASSEMBS := ${SOURCES:.c=.s} ${GENERICS:.c=.s}
DEPENDS := ${OBJECTS:.o=.dep}
MEMBERS := ${patsubst %.o, ${TARGET}(%.o),${OBJECTS}}

EFUNCS =					\
	load					\
	store					\
	exchange				\
	compare_exchange

RFUNCS = load_1					\
	store_1					\
	exchange_1				\
	compare_exchange_1			\
	fetch_add_1				\
	fetch_sub_1				\
	fetch_and_1				\
	fetch_xor_1				\
	fetch_or_1				\
	load_2					\
	store_2					\
	exchange_2				\
	compare_exchange_2			\
	fetch_add_2				\
	fetch_sub_2				\
	fetch_and_2				\
	fetch_xor_2				\
	fetch_or_2				\
	load_4					\
	store_4					\
	exchange_4				\
	compare_exchange_4			\
	fetch_add_4				\
	fetch_sub_4				\
	fetch_and_4				\
	fetch_xor_4				\
	fetch_or_4				\
	load_8					\
	store_8					\
	exchange_8				\
	compare_exchange_8			\
	fetch_add_8				\
	fetch_sub_8				\
	fetch_and_8				\
	fetch_xor_8				\
	fetch_or_8				\
	load_16					\
	store_16				\
	exchange_16				\
	compare_exchange_16			\
	fetch_add_16				\
	fetch_sub_16				\
	fetch_and_16				\
	fetch_xor_16				\
	fetch_or_16

SFUNCS = bool_compare_and_swap_1		\
	val_compare_and_swap_1			\
	fetch_and_add_1				\
	fetch_and_sub_1				\
	fetch_and_and_1				\
	fetch_and_xor_1				\
	fetch_and_or_1				\
	bool_compare_and_swap_2			\
	val_compare_and_swap_2			\
	fetch_and_add_2				\
	fetch_and_sub_2				\
	fetch_and_and_2				\
	fetch_and_xor_2				\
	fetch_and_or_2				\
	bool_compare_and_swap_4			\
	val_compare_and_swap_4			\
	fetch_and_add_4				\
	fetch_and_sub_4				\
	fetch_and_and_4				\
	fetch_and_xor_4				\
	fetch_and_or_4				\
	bool_compare_and_swap_8			\
	val_compare_and_swap_8			\
	fetch_and_add_8				\
	fetch_and_sub_8				\
	fetch_and_and_8				\
	fetch_and_xor_8				\
	fetch_and_or_8				\
	bool_compare_and_swap_16		\
	val_compare_and_swap_16			\
	fetch_and_add_16			\
	fetch_and_sub_16			\
	fetch_and_and_16			\
	fetch_and_xor_16			\
	fetch_and_or_16

.INTERMEDIATE :  ${GENERICS:.c=-tmp.o} redefine_syms.txt defsyms.txt

LDOPTS := ${shell echo ${EFUNCS} | sed 's/\([a-z0-9_][a-z0-9_]*\)/ --defsym=__atomic_\1=__atomic_\1_internal /g'}

OBJOPTS :=  --redefine-syms=redefine_syms.txt

COPTS ?= -O3 -march=native

CFLAGS ?= -Wall -Wno-shadow -isystem `pwd`  -fno-common -fdata-sections -ffunction-sections ${COPTS}

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

redefine_syms.txt : Makefile
	@echo -n ${RFUNCS} | sed 's/\([a-z0-9_][a-z0-9_]*\) */__atomic_\1_internal __atomic_\1\n/g' > $@
	@echo -n ${SFUNCS} | sed 's/\([a-z0-9_][a-z0-9_]*\) */__sync_\1_internal __sync_\1\n/g'     >>$@

defsyms.txt : Makefile
	echo -n ${EFUNCS} | sed 's/\([a-z0-9_][a-z0-9_]*\) */__atomic_\1=__atomic_\1_internal\n/g' > $@


atomic_generic-tmp.o : atomic_generic.c
	${CC} -c ${CFLAGS} -o atomic_generic-tmp.o atomic_generic.c

atomic_generic.o : atomic_generic-tmp.o
	${LD} -r ${LDOPTS} atomic_generic-tmp.o -o atomic_generic.o

atomic_generic_1-tmp.o : atomic_generic_1.c
	${CC} -c ${CFLAGS} -o atomic_generic_1-tmp.o atomic_generic_1.c

atomic_generic_1.o : atomic_generic_1-tmp.o redefine_syms.txt
	 objcopy -v ${OBJOPTS} atomic_generic_1-tmp.o atomic_generic_1.o

atomic_generic_2-tmp.o : atomic_generic_2.c
	${CC} -c ${CFLAGS} -o atomic_generic_2-tmp.o atomic_generic_2.c

atomic_generic_2.o : atomic_generic_2-tmp.o redefine_syms.txt
	 objcopy -v ${OBJOPTS} atomic_generic_2-tmp.o atomic_generic_2.o

atomic_generic_4-tmp.o : atomic_generic_4.c
	${CC} -c ${CFLAGS} -o atomic_generic_4-tmp.o atomic_generic_4.c

atomic_generic_4.o : atomic_generic_4-tmp.o redefine_syms.txt
	 objcopy -v ${OBJOPTS} atomic_generic_4-tmp.o atomic_generic_4.o

atomic_generic_8-tmp.o : atomic_generic_8.c
	${CC} -c ${CFLAGS} -o atomic_generic_8-tmp.o atomic_generic_8.c

atomic_generic_8.o : atomic_generic_8-tmp.o redefine_syms.txt
	 objcopy -v ${OBJOPTS} atomic_generic_8-tmp.o atomic_generic_8.o

atomic_generic_16-tmp.o : atomic_generic_16.c
	${CC} -c ${CFLAGS} -o atomic_generic_16-tmp.o atomic_generic_16.c

atomic_generic_16.o : atomic_generic_16-tmp.o redefine_syms.txt
	 objcopy -v ${OBJOPTS} atomic_generic_16-tmp.o atomic_generic_16.o


%.s : %.c
	${CC} -S ${CFLAGS} $*.c

%.dep : %.c
	@${CC} -M ${CFLAGS} -o $@ $*.c
	@${CC} -M ${CFLAGS} $*.c | sed 's/[.]o\>/.s/g' >> $@

clean :
	rm -fr ${OBJECTS} ${DEPENDS} ${ASSEMBS} libatomic.a

beautify :
	astyle --options=.astylerc *.c *.h

musl : ${SOURCES} ${GENERICS} redefine_syms.txt defsyms.txt
	-mkdir ${MUSL}/src/stdatomic 2> /dev/null || true
	cp ${SOURCES} ${GENERICS} defsyms.txt redefine_syms.txt ${MUSL}/src/stdatomic/
	cp atomic*.h  ${MUSL}/src/internal/

-include ${DEPENDS}
