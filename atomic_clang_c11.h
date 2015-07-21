#ifndef _STDATOMIC_CLANG_C11_H_
#define _STDATOMIC_CLANG_C11_H_ 1

#include <atomic_generic.h>

#define ATOMIC_VAR_INIT(...) __VA_ARGS__
#define atomic_init __c11_atomic_init

/* Map operations to the special builtins that clang provides. */

/* Map all non-explicit macros to the builtin with forced memory order. */
#define atomic_fetch_add(X, Y)                  __c11_atomic_fetch_add((X), (Y), memory_order_seq_cst)
#define atomic_fetch_sub(X, Y)                  __c11_atomic_fetch_sub((X), (Y), memory_order_seq_cst)
#define atomic_fetch_and(X, Y)                  __c11_atomic_fetch_and((X), (Y), memory_order_seq_cst)
#define atomic_fetch_or(X, Y)                   __c11_atomic_fetch_or((X), (Y), memory_order_seq_cst)
#define atomic_fetch_xor(X, Y)                  __c11_atomic_fetch_xor((X), (Y), memory_order_seq_cst)
#define atomic_load(X)                          __c11_atomic_load((X), memory_order_seq_cst)
#define atomic_store(X, V)                      __c11_atomic_store((X), (V), memory_order_seq_cst)
#define atomic_exchange(X, V)                   __c11_atomic_exchange((X), (V), memory_order_seq_cst)
#define atomic_compare_exchange_weak(X, E, V)   __c11_atomic_compare_exchange_weak((X), (E), (V), memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_strong(X, E, V) __c11_atomic_compare_exchange_strong((X), (E), (V), memory_order_seq_cst, memory_order_seq_cst)

/* Map allexplicit macros to the corresponding builtin. */
#define atomic_fetch_add_explicit                                  __c11_atomic_fetch_add
#define atomic_fetch_sub_explicit                                  __c11_atomic_fetch_sub
#define atomic_fetch_and_explicit                                  __c11_atomic_fetch_and
#define atomic_fetch_or_explicit                                   __c11_atomic_fetch_or
#define atomic_fetch_xor_explicit                                  __c11_atomic_fetch_xor
#define atomic_load_explicit(X, MO)                                __c11_atomic_load
#define atomic_store_explicit(X, V, MO)                            __c11_atomic_store
#define atomic_exchange_explicit(X, V, MO)                         __c11_atomic_exchange
#define atomic_compare_exchange_strong_explicit(X, E, V, MOS, MOF) __c11_atomic_compare_exchange

#define INSTANTIATE_STUB_LF(N, T)                                       \
T atomic_fetch_add_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  return __c11_atomic_fetch_add(_X, _V, _mo);                           \
}                                                                       \
T atomic_fetch_sub_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  return __c11_atomic_fetch_sub(_X, _V, _mo);                           \
}                                                                       \
T atomic_fetch_and_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  return __c11_atomic_fetch_and(_X, _V, _mo);                           \
}                                                                       \
T atomic_fetch_xor_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  return __c11_atomic_fetch_xor(_X, _V, _mo);                           \
}                                                                       \
T atomic_fetch_or_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  return __c11_atomic_fetch_or(_X, _V, _mo);                            \
}                                                                       \
T atomic_load_ ## N ## _internal(_Atomic(T)* _X, int _mo) {             \
  return __c11_atomic_load(_X, _mo);                                  \
}                                                                       \
void atomic_store_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  __c11_atomic_store(_X, _V, _mo);                                    \
}                                                                       \
T atomic_exchange_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  return __c11_atomic_exchange(_X, _V, _mo);                          \
}                                                                       \
_Bool atomic_compare_exchange_ ## N ## _internal(_Atomic(T)* _X, T* _E, T const _V, int _mos, int _mof) { \
  return __c11_atomic_compare_exchange_strong(_X, _E, _V, _mos, _mof);    \
}

#define INSTANTIATE_STUB_LC(N, T)                                       \
T atomic_fetch_add_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T exp = 0;                                                            \
  T rep = _V;                                                           \
  int mof = _mo == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!atomic_compare_exchange_internal(N, _X, &exp, &rep, _mo, mof)){ \
    rep = exp + _V;                                                     \
  }                                                                     \
  return exp;                                                           \
}                                                                       \
T atomic_fetch_sub_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T exp = 0;                                                            \
  T rep = _V;                                                           \
  int mof = _mo == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!atomic_compare_exchange_internal(N, _X, &exp, &rep, _mo, mof)){ \
    rep = exp - _V;                                                     \
  }                                                                     \
  return exp;                                                           \
}                                                                       \
T atomic_fetch_and_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T exp = 0;                                                            \
  T rep = _V;                                                           \
  int mof = _mo == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!atomic_compare_exchange_internal(N, _X, &exp, &rep, _mo, mof)){ \
    rep = exp & _V;                                                     \
  }                                                                     \
  return exp;                                                           \
}                                                                       \
T atomic_fetch_xor_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T exp = 0;                                                            \
  T rep = _V;                                                           \
  int mof = _mo == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!atomic_compare_exchange_internal(N, _X, &exp, &rep, _mo, mof)){ \
    rep = exp ^ _V;                                                     \
  }                                                                     \
  return exp;                                                           \
}                                                                       \
T atomic_fetch_or_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T exp = 0;                                                            \
  T rep = _V;                                                           \
  int mof = _mo == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!atomic_compare_exchange_internal(N, _X, &exp, &rep, _mo, mof)){ \
    rep = exp | _V;                                                     \
  }                                                                     \
  return exp;                                                           \
}                                                                       \
T atomic_load_ ## N ## _internal(_Atomic(T)* _X, int _mo) {             \
  T ret;                                                                \
  atomic_load_internal(N, _X, &ret, _mo);                               \
  return ret;                                                           \
}                                                                       \
void atomic_store_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  atomic_store_internal(N, _X, &_V, _mo);                               \
}                                                                       \
T atomic_exchange_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T ret;                                                                \
  atomic_exchange_internal(N, _X, &_V, &ret, _mo);                      \
  return ret;                                                           \
}                                                                       \
_Bool atomic_compare_exchange_ ## N ## _internal(_Atomic(T)* _X, T* _E, T const _V, int _mos, int _mof) { \
  return atomic_compare_exchange_internal(N, _X, _E, &_V, _mos, _mof);  \
}


#define INSTANTIATE_STUB(N, T) INSTANTIATE_STUB_ ## N(T)

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1
# define INSTANTIATE_STUB_1(T) INSTANTIATE_STUB_LF(1, T)
#else
# define INSTANTIATE_STUB_1(T) INSTANTIATE_STUB_LC(1, T)
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2
# define INSTANTIATE_STUB_2(T) INSTANTIATE_STUB_LF(2, T)
#else
# define INSTANTIATE_STUB_2(T) INSTANTIATE_STUB_LC(2, T)
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
# define INSTANTIATE_STUB_4(T) INSTANTIATE_STUB_LF(4, T)
#else
# define INSTANTIATE_STUB_4(T) INSTANTIATE_STUB_LC(4, T)
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
# define INSTANTIATE_STUB_8(T) INSTANTIATE_STUB_LF(8, T)
#else
# define INSTANTIATE_STUB_8(T) INSTANTIATE_STUB_LC(8, T)
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16
# define INSTANTIATE_STUB_16(T) INSTANTIATE_STUB_LF(16, T)
#else
# define INSTANTIATE_STUB_16(T) INSTANTIATE_STUB_LC(16, T)
#endif


#endif
