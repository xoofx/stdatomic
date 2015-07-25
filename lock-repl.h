#ifndef _STDATOMIC_ATOMIC_LOCK_H_
#define _STDATOMIC_ATOMIC_LOCK_H_ 1

#include <atomic_flag.h>

void __impl_mut_lock(_Atomic(int) volatile* f);
void __impl_mut_unlock(_Atomic(int) volatile* f);

/* make the use similar to musl */
#define LOCK(L) __impl_mut_lock((_Atomic(int) volatile*)L)
#define UNLOCK(L) __impl_mut_unlock((_Atomic(int) volatile*)L)

#endif
