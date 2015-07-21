#ifndef _STDATOMIC_ATOMIC_GENERIC_H_
#define _STDATOMIC_ATOMIC_GENERIC_H_ 1

void atomic_load_internal (size_t size, void* ptr, void* ret, int mo);
void atomic_store_internal (size_t size, void* ptr, void const* val, int mo);
void atomic_exchange_internal (size_t size, void*__restrict__ ptr, void const* val, void* ret, int mo);
_Bool atomic_compare_exchange_internal (size_t size, void* ptr, void* expected, void const* desired, int mos, int mof);
void atomic_print_stat(void);

#endif
