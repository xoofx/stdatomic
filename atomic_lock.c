#include "atomic_fence.h"
#include "atomic_lock.h"

void (atomic_lock_unlock)(volatile atomic_lock* f){
  atomic_flag_clear_explicit(f, memory_order_release);
}

void (atomic_lock_lock)(volatile atomic_lock* f){
  do {
    // busy loop
  } while (atomic_flag_test_and_set_explicit(f, memory_order_consume));
  atomic_thread_fence(memory_order_acq_rel);
}
