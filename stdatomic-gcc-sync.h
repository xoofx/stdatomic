#ifndef _STDATOMIC_GCC_SYNC_H_
#define _STDATOMIC_GCC_SYNC_H_ 1

#include "atomic_constants.h"
#include "atomic_flag.h"
#include "atomic_generic.h"
#include <stdint.h>
#include <stdlib.h>


/* There is no compiler support for _Atomic type qualification, so we
   use the type specifier variant. The idea is to use a one element
   array to ensure that such an _Atomic(something) can never be used
   in operators.

   Underneath we will use uintXX_t for special cases. To be sure that
   no bad things can happen, then, we ensure that the alignment for
   these special cases is as wide as possible, namely sizeof the
   type. */

#define _Chose_align(T)                         \
(sizeof(T) == 1 ? 1                             \
 : (sizeof(T) == 2 ? 2                          \
    : (sizeof(T) == 4 ? 4                       \
       : ((sizeof(T) == 8) ? 8                  \
          : ((sizeof(T) == 16) ? 16             \
             : __alignof__(T))))))

#define _Atomic(T) __typeof__(__attribute__ ((__aligned__(_Chose_align(T)))) __typeof__(T)[1])

#define ATOMIC_VAR_INIT(L) { [0] = L, }

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

#define atomic_compare_exchange_weak(X, E, D) atomic_compare_exchange_strong((X), (E), (V))

#define INSTANTIATE_CAS(N, T)                                           \
extern inline                                                           \
_Bool atomic_compare_exchange_strong_ ## N(void*__restrict__ _X, void*__restrict__ _E, void const*__restrict__ _D); \
extern inline                                                           \
void atomic_load_ ## N(void*__restrict__ _X, void*__restrict__ _E);     \
extern inline                                                           \
void atomic_exchange_ ## N(void*__restrict__ _X, void const*__restrict__ _V, void* _R); \
 extern inline                                                          \
void atomic_store_ ## N(void*__restrict__ _X, void const* _V)

#define DECLARE_CAS_SYNC(N, T)                                          \
inline                                                                  \
_Bool atomic_compare_exchange_strong_ ## N(void*__restrict__ _X, void*__restrict__ _E, void const*__restrict__ _D) { \
  T* _x = _X;                                                           \
  T* _e = _E;                                                           \
  T const* _d = _D;                                                     \
  T _v = *_e;                                                           \
  T _n = __sync_val_compare_and_swap(_x, _v, *_d);                      \
  register _Bool _r = (_v == _n);                                       \
  if (!_r) *_e = _n;                                                    \
  return _r;                                                            \
}                                                                       \
                                                                        \
inline                                                                  \
void atomic_load_ ## N(void*__restrict__ _X, void*__restrict__ _E) {    \
  T* _x = _X;                                                           \
  T* _e = _E;                                                           \
  *_e = __sync_val_compare_and_swap(_x, (T)0, (T)0);                    \
}                                                                       \
                                                                        \
inline                                                                  \
 void atomic_exchange_ ## N(void*__restrict__ _X, void const*__restrict__ _V, void* _R) { \
  T* _x = _X;                                                           \
  T const* _v = _V;                                                     \
  T* _r = _R;                                                           \
  T _e = *_v;                                                           \
  for (;;) {                                                            \
    T _n = __sync_val_compare_and_swap(_x, _e, *_v);                    \
    if (_n == _e) {                                                     \
      *_r = _n;                                                         \
      break;                                                            \
    }                                                                   \
    _e = _n;                                                            \
  }                                                                     \
}                                                                       \
                                                                        \
inline                                                                  \
 void atomic_store_ ## N(void*__restrict__ _X, void const* _V) {        \
  T* _x = _X;                                                           \
  T const* _v = _V;                                                     \
  T _e = *_v;                                                           \
  for (;;) {                                                            \
    T _n = __sync_val_compare_and_swap(_x, _e, *_v);                    \
    if (_n == _e) {                                                     \
      break;                                                            \
    }                                                                   \
    _e = _n;                                                            \
  }                                                                     \
}

/* Here the parameter T is there only for compatibility with the
   previous */
#define DECLARE_CAS_GENERIC(N, T)                                       \
                                                                        \
inline                                                                  \
_Bool atomic_compare_exchange_strong_ ## N(void*__restrict__ _X, void*__restrict__ _E, void const*__restrict__ _D) { \
  return atomic_compare_exchange_internal(N, _X, _E, _D, 0, memory_order_seq_cst, memory_order_seq_cst); \
}                                                                       \
                                                                        \
inline                                                                  \
void atomic_load_ ## N(void*__restrict__ _X, void*__restrict__ _E) {    \
  atomic_load_internal(N, _X, _E, memory_order_seq_cst);                \
}                                                                       \
                                                                        \
inline                                                                  \
void atomic_exchange_ ## N(void*__restrict__ _X, void const*__restrict__ _V, void* _R) { \
  atomic_exchange_internal(N, _X, _V, _R, memory_order_seq_cst);        \
}                                                                       \
                                                                        \
inline                                                                  \
void atomic_store_ ## N(void*__restrict__ _X, void const* _V) {         \
  atomic_store_internal(N, _X, _V, memory_order_seq_cst);               \
}

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1
DECLARE_CAS_SYNC(1, uint8_t);
#else
DECLARE_CAS_GENERIC(1, uint8_t);
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2
DECLARE_CAS_SYNC(2, uint16_t);
#else
DECLARE_CAS_GENERIC(2, uint16_t);
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
DECLARE_CAS_SYNC(4, uint32_t);
#else
DECLARE_CAS_GENERIC(4, uint32_t);
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
DECLARE_CAS_SYNC(8, uint64_t);
#else
DECLARE_CAS_GENERIC(8, uint64_t);
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16
DECLARE_CAS_SYNC(16, __uint128_t);
#else
DECLARE_CAS_GENERIC(16, __uint128_t);
#endif

#define atomic_compare_exchange_strong(X, E, D)                         \
({                                                                      \
  _Bool ret;                                                            \
  __typeof__((*X)[0]) const _d = (D);                                   \
  switch (sizeof _d) {                                                  \
  case 8: ret = atomic_compare_exchange_strong_8((X), (E), &_d); break; \
  case 4: ret = atomic_compare_exchange_strong_4((X), (E), &_d); break; \
  case 2: ret = atomic_compare_exchange_strong_2((X), (E), &_d); break; \
  case 1: ret = atomic_compare_exchange_strong_1((X), (E), &_d); break; \
  default: ret = atomic_compare_exchange_internal(sizeof _d, (X), (E), &_d, 0, memory_order_seq_cst, memory_order_seq_cst); \
  }                                                                     \
  __aret(ret);                                                          \
 })

#define atomic_load(X)                                                  \
({                                                                      \
  __typeof__((*X)[0]) _r;                                               \
  switch (sizeof _r) {                                                  \
  case 8: atomic_load_8((X), &_r); break;                               \
  case 4: atomic_load_4((X), &_r); break;                               \
  case 2: atomic_load_2((X), &_r); break;                               \
  case 1: atomic_load_1((X), &_r); break;                               \
  default: atomic_load_internal(sizeof _r, (&(*X)[0]), &_r, memory_order_seq_cst); \
  }                                                                     \
  __aret(_r);                                                           \
 })

#define atomic_exchange(X, D)                                           \
({                                                                      \
  __typeof__((*X)[0]) _r;                                               \
  __typeof__((*X)[0]) const _d = (D);                                   \
  switch (sizeof _d) {                                                  \
  case 8: atomic_load_8((X), &_d, &_r); break;                          \
  case 4: atomic_load_4((X), &_d, &_r); break;                          \
  case 2: atomic_load_2((X), &_d, &_r); break;                          \
  case 1: atomic_load_1((X), &_d, &_r); break;                          \
  default: atomic_exchange_internal(sizeof _d, (&(*X)[0]), &_d, &_r, memory_order_seq_cst); \
  }                                                                     \
  __aret(_r);                                                           \
 })

#define atomic_store(X)                                                 \
({                                                                      \
  __typeof__((*X)[0]) const _d = (D);                                   \
  switch (sizeof _d) {                                                  \
  case 8: atomic_load_8((X), &_d); break;                               \
  case 4: atomic_load_4((X), &_d); break;                               \
  case 2: atomic_load_2((X), &_d); break;                               \
  case 1: atomic_load_1((X), &_d); break;                               \
  default: atomic_store_internal(sizeof _d, (&(*X)[0]), &_d, memory_order_seq_cst); \
  }                                                                     \
 })

#endif
