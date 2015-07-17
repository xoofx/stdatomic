#ifndef _STDATOMIC_ATOMIC_LOCK_H_
#define _STDATOMIC_ATOMIC_LOCK_H_ 1

#include "atomic_flag.h"

typedef atomic_flag atomic_lock;

void atomic_lock_lock(volatile atomic_lock*);
void atomic_lock_unlock(volatile atomic_lock*);

#define atomic_lock_unlock(F) atomic_flag_clear_explicit((F), memory_order_release)

#define atomic_lock_lock(F)                                             \
({                                                                      \
  atomic_lock volatile* atomic_lock_lock = (F);                         \
  do {                                                                  \
    /* busy loop */                                                     \
  } while (atomic_flag_test_and_set_explicit(atomic_lock_lock, memory_order_acquire)); \
 })


#endif
