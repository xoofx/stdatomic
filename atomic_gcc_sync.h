#ifndef _STDATOMIC_GCC_SYNC_H_
#define _STDATOMIC_GCC_SYNC_H_ 1

#include <atomic_constants.h>
#include <atomic_flag.h>
#include <atomic_generic.h>
#include <atomic_stub.h>
#include <stdint.h>
#include <stdlib.h>


#define ATOMIC_VAR_INIT(...) { [0] = __VA_ARGS__, }
#define atomic_init(X, V) ((void)((*(X))[0]=(V)))

/* Map all non-explicit macros to the explicit version. */
#define atomic_fetch_add(X, Y)                  atomic_fetch_add_explicit((X), (Y), memory_order_seq_cst)
#define atomic_fetch_sub(X, Y)                  atomic_fetch_sub_explicit((X), (Y), memory_order_seq_cst)
#define atomic_fetch_and(X, Y)                  atomic_fetch_and_explicit((X), (Y), memory_order_seq_cst)
#define atomic_fetch_or(X, Y)                   atomic_fetch_or_explicit((X), (Y), memory_order_seq_cst)
#define atomic_fetch_xor(X, Y)                  atomic_fetch_xor_explicit((X), (Y), memory_order_seq_cst)
#define atomic_load(X)                          atomic_load_explicit((X), memory_order_seq_cst)
#define atomic_store(X, V)                      atomic_store_explicit((X), (V), memory_order_seq_cst)
#define atomic_exchange(X, V)                   atomic_exchange_explicit((X), (V), memory_order_seq_cst)
#define atomic_compare_exchange_weak(X, E, V)   atomic_compare_exchange_strong_explicit((X), (E), (V), memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_strong(X, E, V) atomic_compare_exchange_strong_explicit((X), (E), (V), memory_order_seq_cst, memory_order_seq_cst)

/* The argument X is supposed to be pointer to a one element array of
   the base type. In evaluation context ``*(X)'' decays to a pointer
   to the base type. In __typeof__ context we have to use
   ``&(*(X))[0]'' for that. */
#define atomic_fetch_add_explicit(X, Y, MO) __sync_fetch_and_add(*(X), (Y))
#define atomic_fetch_sub_explicit(X, Y, MO) __sync_fetch_and_sub(*(X), (Y))
#define atomic_fetch_and_explicit(X, Y, MO) __sync_fetch_and_or(*(X), (Y))
#define atomic_fetch_or_explicit(X, Y, MO) __sync_fetch_and_and(*(X), (Y))
#define atomic_fetch_xor_explicit(X, Y, MO) __sync_fetch_and_xor(*(X), (Y))

#define atomic_compare_exchange_weak(X, E, D, MOS, MOF) atomic_compare_exchange_strong((X), (E), (V), (MOS), (MOF))

#define INSTANTIATE_STUB_LF(N, T)                                       \
T __atomic_fetch_add_ ## N ## _internal(__typeof__(T volatile[1])* X, T const _V, int _mo) { \
  return __sync_fetch_and_add(&((*X)[0]), _V);                          \
}                                                                       \
T __atomic_fetch_sub_ ## N ## _internal(__typeof__(T volatile[1])* X, T const _V, int _mo) { \
  return __sync_fetch_and_sub(&((*X)[0]), _V);                          \
}                                                                       \
T __atomic_fetch_and_ ## N ## _internal(__typeof__(T volatile[1])* X, T const _V, int _mo) { \
  return __sync_fetch_and_and(&((*X)[0]), _V);                          \
}                                                                       \
T __atomic_fetch_xor_ ## N ## _internal(__typeof__(T volatile[1])* X, T const _V, int _mo) { \
  return __sync_fetch_and_xor(&((*X)[0]), _V, _mo);                     \
}                                                                       \
T __atomic_fetch_or_ ## N ## _internal(__typeof__(T volatile[1])* X, T const _V, int _mo) { \
  return __sync_fetch_and_or(&((*X)[0]), _V, _mo);                      \
}                                                                       \
T __atomic_load_ ## N ## _internal(__typeof__(T volatile[1])* X, int _mo) {            \
  return __sync_val_compare_and_swap(&((*X)[0]), 0, 0);                 \
}                                                                       \
T __atomic_exchange_ ## N ## _internal(__typeof__(T volatile[1])* X, T const _V, int _mo) { \
  T _r = _V, _e;                                                        \
  do {                                                                  \
    _e = _r;                                                            \
    _r = __sync_val_compare_and_swap(&((*X)[0]), _e, _V);               \
  } while (_r != _e);                                                   \
  return _r;                                                            \
}                                                                       \
void __atomic_store_ ## N ## _internal(__typeof__(T volatile[1])* X, T const _V, int _mo) { \
  (void)__atomic_exchange_ ## N ## _internal(X, _V, _mo);      \
}                                                                       \
_Bool __atomic_compare_exchange_ ## N ## _internal(__typeof__(T volatile[1])* X, T* _E, T const _D, int _mos, int _mof) { \
  T _v = *_E;                                                           \
  T _n = __sync_val_compare_and_swap(&((*X)[0]), _v, _D);               \
  if (_v != _n) {                                                       \
    *_E = _n;                                                           \
    return 0;                                                           \
  }                                                                     \
  return 1;                                                             \
}

#define INSTANTIATE_STUB(N, T) INSTANTIATE_STUB_ ## N(T)

#define atomic_compare_exchange_strong_explicit(X, E, D, MOS, MOF)      \
({                                                                      \
  _Bool ret;                                                            \
  __typeof__((*X)[0]) const _d = (D);                                   \
  switch (sizeof _d) {                                                  \
  case 8: ret = __sync_val_compare_and_swap((uint64_t*)(X), (E), (D)); break; \
  case 4: ret = __sync_val_compare_and_swap((uint32_t*)(X), (E), (D)); break; \
  case 2: ret = __sync_val_compare_and_swap((uint16_t*)(X), (E), (D)); break; \
  case 1: ret = __sync_val_compare_and_swap((uint8_t*)(X), (E), (D)); break; \
  default: ret = __atomic_compare_exchange_internal(sizeof (*X), (void*)(X), (E), &_d, MOS, MOS); \
  }                                                                     \
  __aret(ret);                                                          \
 })

#define __atomic_union(T, X) union { __typeof__(*(X)) x; T t; }
#define __atomic_union2T(T, X) (((__atomic_union(T, X)){ .x = (*(X)), }).t)
#define __atomic_union2X(T, X, V) (((__atomic_union(T, X)){ .t = (V), }).x)

#define __atomic_load_union(T, X)                                       \
__atomic_union2X(T, X, __sync_val_compare_and_swap((T*)X, 0, 0))

#define __atomic_exchange_union(T, X, V)                                \
({                                                                      \
  __atomic_union(T, X) _V = { .t = (V), };                              \
  T _r = _V.t, _e;                                                      \
  do {                                                                  \
    _e = _r;                                                            \
    _r = __sync_val_compare_and_swap((T*)X, _e, _V.t);                  \
  } while (_r != _e);                                                   \
  __atomic_union2X(T, X, _r);                                           \
 })

#define __atomic_store_union(T, X, V)                                   \
({                                                                      \
  __atomic_union(T, X) _V = { .t = (V), };                              \
  T _r = _V.t, _e;                                                      \
  do {                                                                  \
    _e = _r;                                                            \
    _r = __sync_val_compare_and_swap((T*)X, _e, _V.t);                  \
  } while (_r != _e);                                                   \
 })

#define __atomic_compare_exchange_union(T, X, E, V)                     \
({                                                                      \
  __typeof__(*E)* _e = (E);                                             \
  __atomic_union(T, X) _V = { .x = (V), };                              \
  __atomic_union(T, X) _E = { .x = *_e, };                              \
  __atomic_union(T, X) _R = { .t = __sync_val_compare_and_swap((T*)X, _E.t, _V.t), }; \
  _Bool _r = (_E.t == _R.t);                                            \
  if (!_r) _E.x = _R.x;                                                 \
  _r;                                                                   \
 })

#define atomic_load_explicit(X, MO)                             \
__builtin_choose_expr                                           \
(                                                               \
 sizeof(*X)==16,                                                \
 __atomic_load_union(__uint128_t, &((*X)[0])),                  \
__builtin_choose_expr                                           \
(                                                               \
 sizeof(*X)==8,                                                 \
 __atomic_load_union(uint64_t, &((*X)[0])),                     \
__builtin_choose_expr                                           \
(                                                               \
 sizeof(*X)==4,                                                 \
 __atomic_load_union(uint32_t, &((*X)[0])),                     \
 __builtin_choose_expr                                          \
(                                                               \
 sizeof(*X)==2,                                                 \
 __atomic_load_union(uint16_t, &((*X)[0])),                     \
 __builtin_choose_expr                                          \
(                                                               \
 sizeof(*X)==1,                                                 \
 __atomic_load_union(uint8_t, &((*X)[0])),                      \
 ({                                                             \
 __typeof__((*X)[0]) _r;                                        \
 __atomic_load_internal(sizeof _r, (void*)(&((*X)[0])), &_r, MO);    \
 _r;                                                            \
 }))))))

#define atomic_store_explicit(X, V, MO)                         \
__builtin_choose_expr                                           \
(                                                               \
 sizeof(*X)==16,                                                \
 __atomic_store_union(__uint128_t, &((*X)[0]), (V)),            \
__builtin_choose_expr                                           \
(                                                               \
 sizeof(*X)==8,                                                 \
 __atomic_store_union(uint64_t, &((*X)[0]), (V)),               \
__builtin_choose_expr                                           \
(                                                               \
 sizeof(*X)==4,                                                 \
 __atomic_store_union(uint32_t, &((*X)[0]), (V)),               \
 __builtin_choose_expr                                          \
(                                                               \
 sizeof(*X)==2,                                                 \
 __atomic_store_union(uint16_t, &((*X)[0]), (V)),               \
 __builtin_choose_expr                                          \
(                                                               \
 sizeof(*X)==1,                                                 \
 __atomic_store_union(uint8_t, &((*X)[0]), (V)),                \
 ({                                                             \
 __typeof__((*X)[0]) const _v = (V);                            \
 __atomic_store_internal(sizeof _v, &((*X)[0]), &_v, MO);       \
 }))))))

#define atomic_exchange_explicit(X, V, MO)                              \
__builtin_choose_expr                                                   \
(                                                                       \
 sizeof(*X)==16,                                                        \
 __atomic_exchange_union(__uint128_t, &((*(X))[0]), (V)),                 \
__builtin_choose_expr                                                   \
(                                                                       \
 sizeof(*X)==8,                                                         \
 __atomic_exchange_union(uint64_t, &((*(X))[0]), (V)),                    \
__builtin_choose_expr                                                   \
(                                                                       \
 sizeof(*X)==4,                                                         \
 __atomic_exchange_union(uint32_t, &((*(X))[0]), (V)),                    \
 __builtin_choose_expr                                                  \
(                                                                       \
 sizeof(*X)==2,                                                         \
 __atomic_exchange_union(uint16_t, &((*(X))[0]), (V)),                    \
 __builtin_choose_expr                                                  \
(                                                                       \
 sizeof(*X)==1,                                                         \
 __atomic_exchange_union(uint8_t, &((*(X))[0]), (V)),                     \
 ({                                                                     \
 __typeof__((*X)[0]) const _v = (V);                                    \
 __typeof__((*X)[0]) _r = (V);                                          \
 __atomic_exchange_internal(sizeof _r, (&((*X)[0])), &_r, &_v, MO);     \
 _r;                                                                    \
 }))))))

#define atomic_compare_exchange_explicit(X, E, V, MOS, MOF)             \
__builtin_choose_expr                                                   \
(                                                                       \
 sizeof(*X)==16,                                                        \
 __atomic_compare_exchange_union(__uint128_t, &((*(X))[0]), (E), (V)),    \
__builtin_choose_expr                                                   \
(                                                                       \
 sizeof(*X)==8,                                                         \
 __atomic_compare_exchange_union(uint64_t, &((*(X))[0]), (E), (V)),       \
__builtin_choose_expr                                                   \
(                                                                       \
 sizeof(*X)==4,                                                         \
 __atomic_compare_exchange_union(uint32_t, &((*(X))[0]), (E), (V)),       \
 __builtin_choose_expr                                                  \
(                                                                       \
 sizeof(*X)==2,                                                         \
 __atomic_compare_exchange_union(uint16_t, &((*(X))[0]), (E), (V)),       \
 __builtin_choose_expr                                                  \
(                                                                       \
 sizeof(*X)==1,                                                         \
 __atomic_compare_exchange_union(uint8_t, &((*(X))[0]), (E), (V)),        \
 ({                                                                     \
 __typeof__((*X)[0]) const _v = (V);                                    \
 __atomic_compare_exchange_internal(sizeof _r, (&((*X)[0])), (E), &_v, MOS, MOF); \
 }))))))

#endif
