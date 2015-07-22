#include "atomic_fence.h"
#include "atomic_lock.h"

void (__atomic_lock_unlock)(volatile __atomic_lock* f){
  __atomic_lock_unlock(f);
}

void (__atomic_lock_lock)(volatile __atomic_lock* f){
  __atomic_lock_lock(f);
}
