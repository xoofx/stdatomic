#ifndef _STDATOMIC_ATOMIC_LOCK_H_
#define _STDATOMIC_ATOMIC_LOCK_H_ 1

#include "atomic_flag.h"

typedef atomic_flag atomic_lock;

void atomic_lock_lock(volatile atomic_lock*);
void atomic_lock_unlock(volatile atomic_lock*);

#endif
