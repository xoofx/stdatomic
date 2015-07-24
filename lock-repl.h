#ifndef _STDATOMIC_ATOMIC_LOCK_H_
#define _STDATOMIC_ATOMIC_LOCK_H_ 1

#include <atomic_flag.h>

void __atomic_lock_lock(_Atomic(int) volatile* f);
void __atomic_lock_unlock(_Atomic(int) volatile* f);

/* make the use similar to musl */
#define LOCK(L) __atomic_lock_lock((_Atomic(int) volatile*)L)
#define UNLOCK(L) __atomic_lock_unlock((_Atomic(int) volatile*)L)

#define weak_alias(A, B) extern  __typeof__(A) B __attribute__((weak,alias(#A)))

#endif
