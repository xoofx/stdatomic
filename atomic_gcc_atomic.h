#ifndef _STDATOMIC_GCC_ATOMIC_H_
#define _STDATOMIC_GCC_ATOMIC_H_ 1

#define ATOMIC_VAR_INIT(...) __VA_ARGS__

/* Map all non-explicit macros to the explicit version. */
#define atomic_fetch_add(X, Y)                  atomic_fetch_add_explicit((X), (Y), memory_order_seq_cst)
#define atomic_fetch_sub(X, Y)                  atomic_fetch_sub_explicit((X), (Y), memory_order_seq_cst)
#define atomic_fetch_and(X, Y)                  atomic_fetch_and_explicit((X), (Y), memory_order_seq_cst)
#define atomic_fetch_or(X, Y)                   atomic_fetch_or_explicit((X), (Y), memory_order_seq_cst)
#define atomic_fetch_xor(X, Y)                  atomic_fetch_xor_explicit((X), (Y), memory_order_seq_cst)
#define atomic_load(X)                          atomic_load_explicit((X), memory_order_seq_cst)
#define atomic_store(X, V)                      atomic_store_explicit((X), (V), memory_order_seq_cst)
#define atomic_exchange(X, V)                   atomic_exchange_explicit((X), (V), memory_order_seq_cst)
#define atomic_compare_exchange_weak(X, E, V)   atomic_compare_exchange_weak_explicit((X), (E), (V), memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_strong(X, E, V) atomic_compare_exchange_strong_explicit((X), (E), (V), memory_order_seq_cst, memory_order_seq_cst)

/* Map allexplicit macros to the corresponding builtin.          */
/* The arithmetic operations don't have to use a memory operand. */
#define atomic_fetch_add_explicit(X, Y, MO) __atomic_fetch_add((X), (Y), (MO))
#define atomic_fetch_sub_explicit(X, Y, MO) __atomic_fetch_sub((X), (Y), (MO))
#define atomic_fetch_and_explicit(X, Y, MO) __atomic_fetch_and((X), (Y), (MO))
#define atomic_fetch_or_explicit(X, Y, MO)  __atomic_fetch_or((X), (Y), (MO))
#define atomic_fetch_xor_explicit(X, Y, MO) __atomic_fetch_xor((X), (Y), (MO))

/* The interfaces for the universal functions need to operate on
   memory operands, only. */

#define atomic_load_explicit(X, MO)             \
({                                              \
  __atyp(*X) _r;                                \
  __atomic_load((X), _r, (MO));                 \
  __aret(_r[0]);                                \
 })

#define atomic_store_explicit(X, V, MO)         \
  __atomic_store((X), __atmp(*X, V), (MO))

#define atomic_exchange_explicit(X, V, MO)              \
({                                                      \
  __atyp(*E) _r[1];                                     \
  __atomic_exchange((X), __atmp(*X, V), _r, (MO));      \
  __aret(_r[0]);                                        \
 })

#define atomic_compare_exchange_weak_explicit(X, E, V, MOS, MOF)        \
  __atomic_compare_exchange((X), (E), __atmp(*E, V), 1, (MOS), (MOF))

#define atomic_compare_exchange_strong_explicit(X, E, V, MOS, MOF)      \
  __atomic_compare_exchange((X), (E), __atmp(*E, V), 0, (MOS), (MOF))

#endif
