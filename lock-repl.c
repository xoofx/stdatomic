#include "stdatomic-impl.h"

void (__atomic_lock_unlock)(_Atomic(int) volatile* f) {
	atomic_store_explicit(f, 0, memory_order_release);
}

void (__atomic_lock_lock)(_Atomic(int) volatile* f) {
	do {
		/* busy loop */
	} while (atomic_compare_exchange_strong_explicit(f, (unsigned[]){ 0 }, 1, memory_order_acq_rel, memory_order_consume));
}
