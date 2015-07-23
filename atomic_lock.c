#include "atomic_fence.h"
#include "atomic_lock.h"

void (__atomic_lock_unlock)(volatile __atomic_lock* f){
  atomic_flag_clear_explicit(&f->l, memory_order_release);
}

void (__atomic_lock_lock)(volatile __atomic_lock* f){
  do {
    /* busy loop */
  } while (atomic_flag_test_and_set_explicit(&f->l, memory_order_acq_rel));
}
