#ifndef _STDATOMIC_ATOMIC_GENERIC_H_
#define _STDATOMIC_ATOMIC_GENERIC_H_ 1

void __atomic_load_internal (size_t size, void volatile* ptr, void volatile* ret, int mo);
void __atomic_store_internal (size_t size, void volatile* ptr, void const volatile* val, int mo);
void __atomic_exchange_internal (size_t size, void volatile*__restrict__ ptr, void const volatile* val, void volatile* ret, int mo);
_Bool __atomic_compare_exchange_internal (size_t size, void volatile* ptr, void volatile* expected, void const volatile* desired, int mos, int mof);
void __atomic_print_stat(void);

#endif
