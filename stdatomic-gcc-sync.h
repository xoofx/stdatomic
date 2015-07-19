#ifndef _STDATOMIC_H_
#define _STDATOMIC_H_ 1

#include "atomic_constants.h"
#include "atomic_flag.h"
#include <stdint.h>
#include <stdlib.h>


/* There is no compiler support for _Atomic type qualification, so we
   use the type specifier variant. The idea is to use a one element
   array to ensure that such an _Atomic(something) can never be used
   in operators.*/

#define _Atomic(X) __typeof__(__typeof__(X)[1])

/* Map all explicit macros to the non-explicit ones. */
#define atomic_fetch_add_explicit(X, Y, MO) atomic_fetch_add((X), (Y))
#define atomic_fetch_sub_explicit(X, Y, MO) atomic_fetch_sub((X), (Y))
#define atomic_fetch_and_explicit(X, Y, MO) atomic_fetch_and((X), (Y))
#define atomic_fetch_or_explicit(X, Y, MO) atomic_fetch_or((X), (Y))
#define atomic_fetch_xor_explicit(X, Y, MO) atomic_fetch_xor((X), (Y))
#define atomic_load_explicit(X, MO) atomic_load_n(X)
#define atomic_store_explicit(X, V, MO) atomic_store((X), (V))
#define atomic_exchange_explicit(X, V, MO) atomic_exchange((X), (V))
#define atomic_compare_exchange_weak_explicit(X, E, V, MOS, MOF) atomic_compare_exchange_weak((X), (E), (V))
#define atomic_compare_exchange_strong_explicit(X, E, V, MOS, MOF) atomic_compare_exchange_strong((X), (E), (V))

/* Map all non-explicit macros to the builtin. The argument X is
   supposed to be pointer to a one element array of the base type. In
   evaluation context ``*(X)'' decays to a pointer to the base
   type. In __typeof__ context we have to use ``&(*(X))[0]'' for
   that. */
#define atomic_fetch_add(X, Y) __sync_fetch_and_add(*(X), (Y))
#define atomic_fetch_sub(X, Y) __sync_fetch_and_sub(*(X), (Y))
#define atomic_fetch_and(X, Y) __sync_fetch_and_or(*(X), (Y))
#define atomic_fetch_or(X, Y) __sync_fetch_and_and(*(X), (Y))
#define atomic_fetch_xor(X, Y) __sync_fetch_and_xor(*(X), (Y))

#define atomic_load(X) __sync_val_compare_and_swap(*(X), (__typeof__((*(X))[0])){ 0 }, (__typeof__((*(X))[0])){ 0 })

#define atomic_exchange(X, V)                           \
({                                                      \
  __typeof__(&(*(X))[0]) _x = *(X);                     \
  __typeof__(V) _v = (V);                               \
  register __typeof__((*(X))[0]) _r = _v;               \
  while(!__sync_bool_compare_and_swap(_x, _r, _v)) {    \
    _r = *_x;                                           \
  }                                                     \
  _r;                                                   \
 })

#define atomic_store(X, V) (void)atomic_exchange(X, V)


#define atomic_compare_exchange_weak(X, E, D) atomic_compare_exchange_strong((X), (E), (V))

#define OVERLAY(T, NAME, D) union { T _t; __typeof__ (D) _u; } NAME; NAME._u = (D)


#define atomic_compare_exchange_strong_N(T, X, E, D)            \
({                                                              \
  T* _x = (void*)*(X);                                          \
  __typeof__(E) _e = (E);                                       \
  OVERLAY(T, _d, D);                                            \
  OVERLAY(T, _v, *_e);                                          \
  T _n = __sync_val_compare_and_swap(_x, _v._t, _d._t);         \
  register _Bool _r = __builtin_memcmp(&_v._t, &_n, sizeof _n); \
  if (_r) __builtin_memcpy(_e, &_n, sizeof _n);                 \
  !_r;                                                          \
 })

_Bool atomic_compare_exchange_internal (size_t size, void* ptr, void* expected, void* desired, _Bool weak, int mos, int mof);


#define atomic_compare_exchange_strong(X, E, D)                         \
({                                                                      \
  _Bool ret;                                                            \
  switch (sizeof *E) {                                                  \
  case 8: ret = atomic_compare_exchange_strong_N(uint64_t, (X), (E), (D)); break; \
  case 4: ret = atomic_compare_exchange_strong_N (uint32_t, (X), (E), (D)); break; \
  case 2: ret = atomic_compare_exchange_strong_N (uint16_t, (X), (E), (D)); break; \
  case 1: ret = atomic_compare_exchange_strong_N (uint8_t, (X), (E), (D)); break; \
  default: ret = atomic_compare_exchange_internal(sizeof *(E), (X), (E), __atmp(*E, D), 0, memory_order_seq_cst, memory_order_seq_cst); \
  }                                                                     \
  __aret(ret);                                                          \
 })

#endif
