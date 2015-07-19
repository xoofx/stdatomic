#ifndef _STDATOMIC_CLANG_C11_H_
#define _STDATOMIC_CLANG_C11_H_ 1

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

#endif
