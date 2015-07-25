#ifndef _STDATOMIC_STUB_H_
#define _STDATOMIC_STUB_H_ 1

#define INSTANTIATE_STUB_LC(N, T)                                       \
T __impl_fetch_add_ ## N(void volatile* X, T const V, int MO) {         \
  T E = 0;                                                              \
  T R = V;                                                              \
  int mof = (MO == memory_order_relaxed                                 \
             ? memory_order_relaxed                                     \
             : memory_order_consume);                                   \
  while (!__impl_compare_exchange(N, X, &E, &R, MO, mof)){              \
    R = E + V;                                                          \
  }                                                                     \
  return E;                                                             \
}                                                                       \
T __impl_fetch_sub_ ## N(void volatile* X, T const V, int MO) {         \
  T E = 0;                                                              \
  T R = V;                                                              \
  int mof = (MO == memory_order_relaxed                                 \
             ? memory_order_relaxed                                     \
             : memory_order_consume);                                   \
  while (!__impl_compare_exchange(N, X, &E, &R, MO, mof)){              \
    R = E - V;                                                          \
  }                                                                     \
  return E;                                                             \
}                                                                       \
T __impl_fetch_and_ ## N(void volatile* X, T const V, int MO) {         \
  T E = 0;                                                              \
  T R = V;                                                              \
  int mof = (MO == memory_order_relaxed                                 \
             ? memory_order_relaxed                                     \
             : memory_order_consume);                                   \
  while (!__impl_compare_exchange(N, X, &E, &R, MO, mof)){              \
    R = E & V;                                                          \
  }                                                                     \
  return E;                                                             \
}                                                                       \
T __impl_fetch_xor_ ## N(void volatile* X, T const V, int MO) {         \
  T E = 0;                                                              \
  T R = V;                                                              \
  int mof = (MO == memory_order_relaxed                                 \
             ? memory_order_relaxed                                     \
             : memory_order_consume);                                   \
  while (!__impl_compare_exchange(N, X, &E, &R, MO, mof)){              \
    R = E ^ V;                                                          \
  }                                                                     \
  return E;                                                             \
}                                                                       \
T __impl_fetch_or_ ## N(void volatile* X, T const V, int MO) {          \
  T E = 0;                                                              \
  T R = V;                                                              \
  int mof = MO == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!__impl_compare_exchange(N, X, &E, &R, MO, mof)){              \
    R = E | V;                                                          \
  }                                                                     \
  return E;                                                             \
}                                                                       \
T __impl_load_ ## N(void volatile* X, int MO) {                         \
  T ret;                                                                \
  __impl_load(N, X, &ret, MO);                                          \
  return ret;                                                           \
}                                                                       \
void __impl_store_ ## N(void volatile* X, T const V, int MO) {          \
  __impl_store(N, X, &V, MO);                                           \
}                                                                       \
T __impl_exchange_ ## N(void volatile* X, T const V, int MO) {          \
  T ret;                                                                \
  __impl_exchange(N, X, &V, &ret, MO);                                  \
  return ret;                                                           \
}                                                                       \
_Bool __impl_compare_exchange_ ## N(void volatile* X, T* E, T const V, int MOS, int MOf) { \
  return __impl_compare_exchange(N, X, E, &V, MOS, MOf);                \
}

#define INSTANTIATE_SYNC(N, T)                                          \
T __impl_fetch_and_add_ ## N(void volatile* X, T const V) {             \
  return __impl_fetch_add_ ## N((_Atomic(T)*)X, V, memory_order_seq_cst); \
}                                                                       \
T __impl_fetch_and_sub_ ## N(void volatile* X, T const V) {             \
  return __impl_fetch_sub_ ## N((_Atomic(T)*)X, V, memory_order_seq_cst); \
}                                                                       \
T __impl_fetch_and_and_ ## N(void volatile* X, T const V) {             \
  return __impl_fetch_and_ ## N((_Atomic(T)*)X, V, memory_order_seq_cst); \
}                                                                       \
T __impl_fetch_and_or_ ## N(void volatile* X, T const V) {              \
  return __impl_fetch_or_ ## N((_Atomic(T)*)X, V, memory_order_seq_cst); \
}                                                                       \
T __impl_fetch_and_xor_ ## N(void volatile* X, T const V) {             \
  return __impl_fetch_xor_ ## N((_Atomic(T)*)X, V, memory_order_seq_cst); \
}                                                                       \
_Bool __impl_bool_compare_and_swap_ ## N(void volatile* X, T E, T const V) { \
  T R = E;                                                              \
  return __impl_compare_exchange_ ## N((_Atomic(T)*)X, &R, V,           \
                                                      memory_order_seq_cst, memory_order_seq_cst); \
}                                                                       \
T __impl_val_compare_and_swap_ ## N(void volatile* X, T E, T const V) { \
   T R = E;                                                             \
  __impl_compare_exchange_ ## N((_Atomic(T)*)X, &R, V,                  \
                                               memory_order_seq_cst, memory_order_seq_cst); \
  return R;                                                             \
}


#endif
