#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <math.h>
#include "stdatomic.h"
#include "atomic_generic.h"

/* the size of this table has a trade off between the probability of
   collisions (the bigger the table, the better) and the waste of
   space (the smaller, the better). */

#ifndef HBIT
# define HBIT 8
#endif
/* len is a power of two such that we just can mask out higher bits */
enum { LEN = 1<<HBIT, };
enum { ptrbit = sizeof(uintptr_t)*CHAR_BIT, };

static atomic_lock table[LEN];

#ifdef HASH_STAT
static _Atomic(size_t) draw[LEN];
#endif

/* Chose a medium sized prime number as a factor. The multiplication
   by it is a bijection modulo any LEN. */
#define MAGIC 14530039U


static
unsigned hash(void* X) {
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

void atomic_load_internal (size_t size, void* ptr, void* ret, int mo) {
  unsigned pos = hash(ptr);
  atomic_lock_lock(table+pos);
  if (mo == memory_order_seq_cst)
    atomic_thread_fence(memory_order_seq_cst);
  __builtin_memcpy(ret, ptr, size);
  atomic_lock_unlock(table+pos);
}

void atomic_store_internal (size_t size, void* ptr, void const* val, int mo) {
  unsigned pos = hash(ptr);
  atomic_lock_lock(table+pos);
  __builtin_memcpy(ptr, val, size);
  if (mo == memory_order_seq_cst)
    atomic_thread_fence(memory_order_seq_cst);
  atomic_lock_unlock(table+pos);
}

static
void atomic_exchange_internal_restrict (size_t size, void*__restrict__ ptr, void const*__restrict__ val, void*__restrict__ ret, int mo) {
  unsigned pos = hash(ptr);
  atomic_lock_lock(table+pos);
  __builtin_memcpy(ret, ptr, size);
  if (mo == memory_order_seq_cst)
    atomic_thread_fence(memory_order_seq_cst);
  __builtin_memcpy(ptr, val, size);
  atomic_lock_unlock(table+pos);
}

void atomic_exchange_internal (size_t size, void*__restrict__ ptr, void const* val, void* ret, int mo) {
  if (val == ret) {
    unsigned char buffer[size];
    atomic_exchange_internal_restrict(size, ptr, val, buffer, mo);
    __builtin_memcpy(ret, buffer, size);
  } else {
    atomic_exchange_internal_restrict(size, ptr, val, ret, mo);
  }
}

_Bool atomic_compare_exchange_internal (size_t size, void* ptr, void* expected, void const* desired, int mos, int mof) {
  unsigned pos = hash(ptr);
  atomic_lock_lock(table+pos);
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
  atomic_lock_unlock(table+pos);
  return ret;
}

void atomic_print_stat(void) {
#ifdef HASH_STAT
  size_t x1 = 0;
  size_t x2 = 0;
  size_t min = -1;
  size_t max = 0;
  for (size_t i = 0; i < LEN; i++) {
    size_t val = atomic_load(&draw[i]);
    x1 += val;
    x2 += val*val;
    if (val < min) min = val;
    if (val > max) max = val;
  }
  double avg1 = (x1+0.0)/LEN;
  double avg2 = (x2+0.0)/LEN;
  double var = avg2 - avg1*avg1;
  fprintf(stderr, "hash utilisation: %zu < %e (+%e) < %zu\n",
          min, avg1, sqrt(var), max);
#else
  fputs("To collect hash statistics about atomics, compile with ``HASH_STAT''\n", stderr);
#endif
}
