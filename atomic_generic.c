#include <limits.h>
#include <string.h>
#include "stdatomic-impl.h"
#include "atomic_generic.h"
#include "libc.h"

#ifdef HASH_STAT
# include <math.h>
# include <stdio.h>
#endif

/* This is compatible with musl's internal locks. */
/* The lock itself must be lock-free, so in general the can only be an
   atomic_flag if we know nothing else about the platform. */

typedef _Atomic(unsigned) __impl_lock;
void __impl_mut_lock_slow(__impl_lock* loc);
void __impl_mut_unlock_slow(__impl_lock* loc);

static unsigned const contrib = ((UINT_MAX/2u)+2u);

#ifdef ATOMIC_INJECT
extern void atomic_inject(void);
extern _Thread_local _Bool atomic_faulty;
#endif

__attribute__((__always_inline__))
static inline
void __impl_mut_lock(__impl_lock* loc)
{
  if (!atomic_compare_exchange_strong_explicit(loc, (unsigned[1]){ 0 }, contrib, memory_order_acq_rel, memory_order_consume))
    __impl_mut_lock_slow(loc);
#ifdef ATOMIC_INJECT
  if (atomic_faulty) atomic_inject();
#endif
}

__attribute__((__always_inline__))
static inline
void __impl_mut_unlock(__impl_lock* loc)
{
  if (contrib != atomic_fetch_sub_explicit(loc, contrib, memory_order_relaxed))
    __impl_mut_unlock_slow(loc);
}

/* the size of this table has a trade off between the probability of
   collisions (the bigger the table, the better) and the waste of
   space (the smaller, the better). */

#ifndef HBIT
# define HBIT 8
#endif
/* len is a power of two such that we just can mask out higher bits */
enum { LEN = 1<<HBIT, };
enum { ptrbit = sizeof(uintptr_t)*CHAR_BIT, };

static __impl_lock table[LEN];

#ifdef HASH_STAT
static _Atomic(size_t) draw[LEN];
#endif

/* Chose a medium sized prime number as a factor. The multiplication
   by it is a bijection modulo any LEN. */
#define MAGIC 14530039U

__attribute__((__unused__))
static
unsigned __impl_hash(void volatile const* X) {
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
#endif
	return x;
}

__attribute__((__unused__))
static
unsigned __impl_jenkins_one_at_a_time_hash(void volatile const* k) {
	union {
		unsigned char b[sizeof k];
		uintptr_t v;
		void volatile const* k;
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
#endif
	return x;
}

__attribute__((__unused__))
static
uintptr_t __impl_mix(void volatile const* x) {
	uintptr_t h = (uintptr_t)x;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	h %= LEN;
#ifdef HASH_STAT
	atomic_fetch_add_explicit(&draw[h], 1, memory_order_relaxed);
#endif
	return h;
}

__attribute__((__unused__))
static
uintptr_t __impl_8(void volatile const* x) {
	uintptr_t h = (uintptr_t)x;
	h ^= (h >> 8);
	h %= LEN;
#ifdef HASH_STAT
	atomic_fetch_add_explicit(&draw[h], 1, memory_order_relaxed);
#endif
	return h;
}

#ifndef __ATOMIC_HASH
# define __ATOMIC_HASH __impl_hash
#endif
#define hash __ATOMIC_HASH


void __impl_load (size_t size, void volatile* ptr, void volatile* ret, int mo) {
	unsigned pos = hash(ptr);
	__impl_mut_lock(&table[pos]);
	if (mo == memory_order_seq_cst)
		atomic_thread_fence(memory_order_seq_cst);
	memcpy((void*)ret, (void*)ptr, size);
	__impl_mut_unlock(&table[pos]);
}

void __impl_store (size_t size, void volatile* ptr, void const volatile* val, int mo) {
	unsigned pos = hash(ptr);
	__impl_mut_lock(&table[pos]);
	memcpy((void*)ptr, (void*)val, size);
	if (mo == memory_order_seq_cst)
		atomic_thread_fence(memory_order_seq_cst);
	__impl_mut_unlock(&table[pos]);
}

static
void atomic_exchange_restrict (size_t size, void volatile*__restrict__ ptr, void const volatile*__restrict__ val, void volatile*__restrict__ ret, int mo) {
	unsigned pos = hash(ptr);
	__impl_mut_lock(&table[pos]);
	memcpy((void*)ret, (void*)ptr, size);
	if (mo == memory_order_seq_cst)
		atomic_thread_fence(memory_order_seq_cst);
	memcpy((void*)ptr, (void*)val, size);
	__impl_mut_unlock(&table[pos]);
}

void __impl_exchange (size_t size, void volatile*__restrict__ ptr, void const volatile* val, void volatile* ret, int mo) {
	if (val == ret) {
		unsigned char buffer[size];
		atomic_exchange_restrict(size, ptr, val, buffer, mo);
		memcpy((void*)ret, buffer, size);
	} else {
		atomic_exchange_restrict(size, ptr, val, ret, mo);
	}
}

_Bool __impl_compare_exchange (size_t size, void volatile* ptr, void volatile* expected, void const volatile* desired, int mos, int mof) {
	unsigned pos = hash(ptr);
	__impl_mut_lock(&table[pos]);
	_Bool ret = !memcmp((void*)ptr, (void*)expected, size);
	if (ret) {
		memcpy((void*)ptr, (void*)desired, size);
		if (mos == memory_order_seq_cst)
			atomic_thread_fence(memory_order_seq_cst);
	} else {
		if (mof == memory_order_seq_cst)
			atomic_thread_fence(memory_order_seq_cst);
		memcpy((void*)expected, (void*)ptr, size);
	}
	__impl_mut_unlock(&table[pos]);
	/* fprintf(stderr, "cas for %p (%zu) at pos %u, %s, exp %p, des %p\n", */
	/*         ptr, size, pos, ret ? "suceeded" : "failed", */
	/*         expected, desired); */
	return ret;
}

/* To collect hash statistics about atomics, compile with
   ``HASH_STAT'' */
void __impl_print_stat(void) {
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
	        LEN, x1, min, avg1, sqrt(var), max);
#endif
}
