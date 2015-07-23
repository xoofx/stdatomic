#ifndef _STDATOMIC_STUB_H_
#define _STDATOMIC_STUB_H_ 1

#define INSTANTIATE_STUB_LC(N, T)                                       \
T __atomic_fetch_add_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T exp = 0;                                                            \
  T rep = _V;                                                           \
  int mof = _mo == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!__atomic_compare_exchange_internal(N, _X, &exp, &rep, _mo, mof)){ \
    rep = exp + _V;                                                     \
  }                                                                     \
  return exp;                                                           \
}                                                                       \
T __atomic_fetch_sub_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T exp = 0;                                                            \
  T rep = _V;                                                           \
  int mof = _mo == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!__atomic_compare_exchange_internal(N, _X, &exp, &rep, _mo, mof)){ \
    rep = exp - _V;                                                     \
  }                                                                     \
  return exp;                                                           \
}                                                                       \
T __atomic_fetch_and_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T exp = 0;                                                            \
  T rep = _V;                                                           \
  int mof = _mo == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!__atomic_compare_exchange_internal(N, _X, &exp, &rep, _mo, mof)){ \
    rep = exp & _V;                                                     \
  }                                                                     \
  return exp;                                                           \
}                                                                       \
T __atomic_fetch_xor_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T exp = 0;                                                            \
  T rep = _V;                                                           \
  int mof = _mo == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!__atomic_compare_exchange_internal(N, _X, &exp, &rep, _mo, mof)){ \
    rep = exp ^ _V;                                                     \
  }                                                                     \
  return exp;                                                           \
}                                                                       \
T __atomic_fetch_or_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T exp = 0;                                                            \
  T rep = _V;                                                           \
  int mof = _mo == memory_order_relaxed ? memory_order_relaxed : memory_order_consume; \
  while (!__atomic_compare_exchange_internal(N, _X, &exp, &rep, _mo, mof)){ \
    rep = exp | _V;                                                     \
  }                                                                     \
  return exp;                                                           \
}                                                                       \
T __atomic_load_ ## N ## _internal(_Atomic(T)* _X, int _mo) {           \
  T ret;                                                                \
  __atomic_load_internal(N, _X, &ret, _mo);                             \
  return ret;                                                           \
}                                                                       \
void __atomic_store_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  __atomic_store_internal(N, _X, &_V, _mo);                             \
}                                                                       \
T __atomic_exchange_ ## N ## _internal(_Atomic(T)* _X, T const _V, int _mo) { \
  T ret;                                                                \
  __atomic_exchange_internal(N, _X, &_V, &ret, _mo);                    \
  return ret;                                                           \
}                                                                       \
_Bool __atomic_compare_exchange_ ## N ## _internal(_Atomic(T)* _X, T* _E, T const _V, int _mos, int _mof) { \
  return __atomic_compare_exchange_internal(N, _X, _E, &_V, _mos, _mof); \
}

#endif
