#ifndef _STDATOMIC_ATOMIC_LOCK_H_
#define _STDATOMIC_ATOMIC_LOCK_H_ 1

#include <atomic_flag.h>

typedef atomic_flag __atomic_lock;

void __atomic_lock_lock(volatile __atomic_lock*);
void __atomic_lock_unlock(volatile __atomic_lock*);

#define __atomic_lock_unlock(F) atomic_flag_clear_explicit((F), memory_order_release)

#define __atomic_lock_lock(F)                                           \
({                                                                      \
  __atomic_lock volatile* __atomic_lock_lock = (F);                     \
  do {                                                                  \
    /* busy loop */                                                     \
  } while (atomic_flag_test_and_set_explicit(__atomic_lock_lock, memory_order_acquire)); \
 })


#endif
