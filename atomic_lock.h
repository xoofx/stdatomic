#ifndef _STDATOMIC_ATOMIC_LOCK_H_
#define _STDATOMIC_ATOMIC_LOCK_H_ 1

#include <atomic_flag.h>

/* This is size compatible with musl's internal locks*/
/* The lock itself must be lock-free, so in general the can only be an
   atomic_flag if we know nothing else about the platform. */
union __atomic_lock {
  atomic_flag l;
  int volatile d[2];
};
typedef union __atomic_lock __atomic_lock;

void __atomic_lock_lock(volatile __atomic_lock*);
void __atomic_lock_unlock(volatile __atomic_lock*);

/* make the use similar to musl */
#define LOCK(L) __atomic_lock_lock(L)
#define UNLOCK(L) __atomic_lock_unlock(L)

#endif
