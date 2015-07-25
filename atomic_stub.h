#ifndef _STDATOMIC_STUB_H_
#define _STDATOMIC_STUB_H_ 1

#define INSTANTIATE_STUB_LC(N, T)                                       \
T __atomic_fetch_add_ ## N ## _internal(void volatile* X, T const V, int MO) { \
  T E = 0;                                                              \
  T R = V;                                                              \
  int mof = (MO == memory_order_relaxed                                 \
             ? memory_order_relaxed                                     \
             : memory_order_consume);                                   \
  while (!__atomic_compare_exchange_internal(N, X, &E, &R, MO, mof)){   \
    R = E + V;                                                          \
  }                                                                     \
  return E;                                                             \
}                                                                       \
T __atomic_fetch_sub_ ## N ## _internal(void volatile* X, T const V, int MO) { \
  T E = 0;                                                              \
  T R = V;                                                              \
  int mof = (MO == memory_order_relaxed                                 \
             ? memory_order_relaxed                                     \
             : memory_order_consume);                                   \
  while (!__atomic_compare_exchange_internal(N, X, &E, &R, MO, mof)){   \
    R = E - V;                                                          \
  }                                                                     \
  return E;                                                             \
}                                                                       \
T __atomic_fetch_and_ ## N ## _internal(void volatile* X, T const V, int MO) { \
  T E = 0;                                                              \
  T R = V;                                                              \
  int mof = (MO == memory_order_relaxed                                 \
             ? memory_order_relaxed                                     \
             : memory_order_consume);                                   \
  while (!__atomic_compare_exchange_internal(N, X, &E, &R, MO, mof)){   \
    R = E & V;                                                          \
  }                                                                     \
  return E;                                                             \
}                                                                       \
T __atomic_fetch_xor_ ## N ## _internal(void volatile* X, T const V, int MO) { \
  T E = 0;                                                              \
  T R = V;                                                              \
  int mof = (MO == memory_order_relaxed                                 \
             ? memory_order_relaxed                                     \
             : memory_order_consume);                                   \
  while (!__atomic_compare_exchange_internal(N, X, &E, &R, MO, mof)){   \
    R = E ^ V;                                                          \
  }                                                                     \
  return E;                                                             \
}                                                                       \
T __atomic_fetch_or_ ## N ## _internal(void volatile* X, T const V, int MO) { \
  T E = 0;                                                              \
  T R = V;                                                              \
  int mof = MO == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!__atomic_compare_exchange_internal(N, X, &E, &R, MO, mof)){   \
    R = E | V;                                                          \
  }                                                                     \
  return E;                                                             \
}                                                                       \
T __atomic_load_ ## N ## _internal(void volatile* X, int MO) {          \
  T ret;                                                                \
  __atomic_load_internal(N, X, &ret, MO);                               \
  return ret;                                                           \
}                                                                       \
void __atomic_store_ ## N ## _internal(void volatile* X, T const V, int MO) { \
  __atomic_store_internal(N, X, &V, MO);                                \
}                                                                       \
T __atomic_exchange_ ## N ## _internal(void volatile* X, T const V, int MO) { \
  T ret;                                                                \
  __atomic_exchange_internal(N, X, &V, &ret, MO);                       \
  return ret;                                                           \
}                                                                       \
_Bool __atomic_compare_exchange_ ## N ## _internal(void volatile* X, T* E, T const V, int MOS, int MOf) { \
  return __atomic_compare_exchange_internal(N, X, E, &V, MOS, MOf);     \
}

#define INSTANTIATE_SYNC(N, T)                                          \
T __sync_fetch_and_add_ ## N ## _internal(void volatile* X, T const V) { \
  return __atomic_fetch_add_ ## N ## _internal((_Atomic(T)*)X, V, memory_order_seq_cst); \
}                                                                       \
T __sync_fetch_and_sub_ ## N ## _internal(void volatile* X, T const V) { \
  return __atomic_fetch_sub_ ## N ## _internal((_Atomic(T)*)X, V, memory_order_seq_cst); \
}                                                                       \
T __sync_fetch_and_and_ ## N ## _internal(void volatile* X, T const V) { \
  return __atomic_fetch_and_ ## N ## _internal((_Atomic(T)*)X, V, memory_order_seq_cst); \
}                                                                       \
T __sync_fetch_and_or_ ## N ## _internal(void volatile* X, T const V) { \
  return __atomic_fetch_or_ ## N ## _internal((_Atomic(T)*)X, V, memory_order_seq_cst); \
}                                                                       \
T __sync_fetch_and_xor_ ## N ## _internal(void volatile* X, T const V) { \
  return __atomic_fetch_xor_ ## N ## _internal((_Atomic(T)*)X, V, memory_order_seq_cst); \
}                                                                       \
_Bool __sync_bool_compare_and_swap_ ## N ## _internal(void volatile* X, T E, T const V) { \
  T R = E;                                                              \
  return __atomic_compare_exchange_ ## N ## _internal((_Atomic(T)*)X, &R, V, \
                                                      memory_order_seq_cst, memory_order_seq_cst); \
}                                                                       \
T __sync_val_compare_and_swap_ ## N ## _internal(void volatile* X, T E, T const V) { \
   T R = E;                                                             \
  __atomic_compare_exchange_ ## N ## _internal((_Atomic(T)*)X, &R, V,   \
                                               memory_order_seq_cst, memory_order_seq_cst); \
  return R;                                                             \
}


#endif
