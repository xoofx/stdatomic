#include "stdatomic-impl.h"

void (__impl_mut_unlock)(_Atomic(int) volatile* f) {
	atomic_store_explicit(f, 0, memory_order_release);
}

void (__impl_mut_lock)(_Atomic(int) volatile* f) {
	do {
		/* busy loop */
	} while (atomic_compare_exchange_strong_explicit(f, (int[1]){ 0 }, 1, memory_order_acq_rel, memory_order_consume));
}
