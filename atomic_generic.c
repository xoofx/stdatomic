#include <limits.h>
#include "stdatomic.h"
#include "atomic_generic.h"

#ifdef HASH_STAT
# include <math.h>
# include <stdio.h>
#endif

/* the size of this table has a trade off between the probability of
   collisions (the bigger the table, the better) and the waste of
   space (the smaller, the better). */

#ifndef HBIT
# define HBIT 8
#endif
/* len is a power of two such that we just can mask out higher bits */
enum { LEN = 1<<HBIT, };
enum { ptrbit = sizeof(uintptr_t)*CHAR_BIT, };

static __atomic_lock table[LEN];

#ifdef HASH_STAT
static _Atomic(size_t) draw[LEN];
static _Atomic(size_t) draws;
#endif

/* Chose a medium sized prime number as a factor. The multiplication
   by it is a bijection modulo any LEN. */
#define MAGIC 14530039U


unsigned __shift_hash(void* X) {
	uintptr_t const len = LEN;
	uintptr_t x = (uintptr_t)X;
	x *= MAGIC;
	/* Be sure to use all bits in the result. */
	if (ptrbit > 8*HBIT)  x ^= (x / (len*len*len*len*len*len*len*len));
	if (ptrbit > 4*HBIT)  x ^= (x / (len*len*len*len));
	if (ptrbit > 2*HBIT)  x ^= (x / (len*len));
	if (ptrbit > 1*HBIT)  x ^= (x / len);
	x %= len;
#ifdef HASH_STAT
	atomic_fetch_add_explicit(&draw[x], 1, memory_order_relaxed);
	atomic_fetch_add_explicit(&draws, 1, memory_order_relaxed);
#endif
	return x;
}

unsigned __jenkins_one_at_a_time_hash(void *k) {
	union {
		unsigned char b[sizeof k];
		uintptr_t v;
		void* k;
	} key = { .k = k, };
	uintptr_t i, x = 0;
	for(i = 0; i < sizeof(uintptr_t); ++i) {
		x += key.b[i];
		x += (x << 10);
		x ^= (x >> 6);
	}
	x += (x << 3);
	x ^= (x >> 11);
	x += (x << 15);
	x %= LEN;
#ifdef HASH_STAT
	atomic_fetch_add_explicit(&draw[x], 1, memory_order_relaxed);
	atomic_fetch_add_explicit(&draws, 1, memory_order_relaxed);
#endif
	return x;
}

uintptr_t __fmix(void* x) {
	uintptr_t h = (uintptr_t)x;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	h %= LEN;
#ifdef HASH_STAT
	atomic_fetch_add_explicit(&draw[h], 1, memory_order_relaxed);
	atomic_fetch_add_explicit(&draws, 1, memory_order_relaxed);
#endif
	return h;
}

uintptr_t __f8(void* x) {
	uintptr_t h = (uintptr_t)x;
	h >>= 8;
	h %= LEN;
#ifdef HASH_STAT
	atomic_fetch_add_explicit(&draw[h], 1, memory_order_relaxed);
	atomic_fetch_add_explicit(&draws, 1, memory_order_relaxed);
#endif
	return h;
}


#define hash __shift_hash


void __atomic_load_internal (size_t size, void* ptr, void* ret, int mo) {
	unsigned pos = hash(ptr);
	LOCK(table+pos);
	if (mo == memory_order_seq_cst)
		atomic_thread_fence(memory_order_seq_cst);
	__builtin_memcpy(ret, ptr, size);
	UNLOCK(table+pos);
}

void __atomic_store_internal (size_t size, void* ptr, void const* val, int mo) {
	unsigned pos = hash(ptr);
	LOCK(table+pos);
	__builtin_memcpy(ptr, val, size);
	if (mo == memory_order_seq_cst)
		atomic_thread_fence(memory_order_seq_cst);
	UNLOCK(table+pos);
}

static
void atomic_exchange_internal_restrict (size_t size, void*__restrict__ ptr, void const*__restrict__ val, void*__restrict__ ret, int mo) {
	unsigned pos = hash(ptr);
	LOCK(table+pos);
	__builtin_memcpy(ret, ptr, size);
	if (mo == memory_order_seq_cst)
		atomic_thread_fence(memory_order_seq_cst);
	__builtin_memcpy(ptr, val, size);
	UNLOCK(table+pos);
}

void __atomic_exchange_internal (size_t size, void*__restrict__ ptr, void const* val, void* ret, int mo) {
	if (val == ret) {
		unsigned char buffer[size];
		atomic_exchange_internal_restrict(size, ptr, val, buffer, mo);
		__builtin_memcpy(ret, buffer, size);
	} else {
		atomic_exchange_internal_restrict(size, ptr, val, ret, mo);
	}
}

_Bool __atomic_compare_exchange_internal (size_t size, void* ptr, void* expected, void const* desired, int mos, int mof) {
	unsigned pos = hash(ptr);
	LOCK(table+pos);
	_Bool ret = !__builtin_memcmp(ptr, expected, size);
	if (ret) {
		__builtin_memcpy(ptr, desired, size);
		if (mos == memory_order_seq_cst)
			atomic_thread_fence(memory_order_seq_cst);
	} else {
		if (mof == memory_order_seq_cst)
			atomic_thread_fence(memory_order_seq_cst);
		__builtin_memcpy(expected, ptr, size);
	}
	UNLOCK(table+pos);
	/* fprintf(stderr, "cas for %p (%zu) at pos %u, %s, exp %p, des %p\n", */
	/*         ptr, size, pos, ret ? "suceeded" : "failed", */
	/*         expected, desired); */
	return ret;
}

/* To collect hash statistics about atomics, compile with
   ``HASH_STAT'' */
void __atomic_print_stat(void) {
#ifdef HASH_STAT
	size_t x1 = 0;
	size_t x2 = 0;
	size_t min = -1;
	size_t max = 0;
	size_t i;
	for (i = 0; i < LEN; i++) {
		size_t val = atomic_load(&draw[i]);
		fprintf(stderr, "\t%zu", val);
		if ((i % 8) == 7) fputc('\n', stderr);
		x1 += val;
		x2 += val*val;
		if (val < min) min = val;
		if (val > max) max = val;
	}
	fputc('\n', stderr);
	double avg1 = (x1+0.0)/LEN;
	double avg2 = (x2+0.0)/LEN;
	double var = avg2 - avg1*avg1;
	fprintf(stderr, "hash utilisation, %d positions, %zu draws: %zu < %e (+%e) < %zu\n",
	        LEN, atomic_load(&draws), min, avg1, sqrt(var), max);
#endif
}
