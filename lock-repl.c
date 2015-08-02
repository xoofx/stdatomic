#include <limits.h>
#include "stdatomic-impl.h"


#define contrib ((UINT_MAX/2u)+2u)

C¯size __impl_total = 0;
C¯size __impl_fast = 0;
C¯size __impl_slow = 0;
C¯size __impl_futex = 0;
C¯size __impl_again = 0;
C¯size __impl_spin = 0;


void __impl_mut_unlock_slow(_Atomic(unsigned)* f) {
  // empty
}

void __impl_mut_lock_slow(_Atomic(unsigned)* f) {
	do {
		/* busy loop */
	} while (!atomic_compare_exchange_strong_explicit(f, (unsigned[1]){ 0 }, contrib, memory_order_acq_rel, memory_order_consume));
}
