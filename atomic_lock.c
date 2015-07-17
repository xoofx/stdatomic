#include "atomic_fence.h"
#include "atomic_lock.h"

void (atomic_lock_unlock)(volatile atomic_lock* f){
  atomic_lock_unlock(f);
}

void (atomic_lock_lock)(volatile atomic_lock* f){
  atomic_lock_lock(f);
}
