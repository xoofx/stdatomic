#include "pthread_impl.h"
#include "stdatomic-impl.h"

/* The HO bit. */
static unsigned const lockbit = (UINT_MAX/2u)+1u;

size_t __impl_total = 0;
size_t __impl_fast = 0;
size_t __impl_slow = 0;
size_t __impl_futex = 0;
size_t __impl_again = 0;
size_t __impl_spin = 0;

#ifdef BENCH
# define ACCOUNT(X, V) (X) += (V)
#else
# define ACCOUNT(X, V) do { } while(0)
#endif

void __impl_mut_lock_slow(_Atomic(unsigned)* loc);
void __impl_mut_unlock_slow(_Atomic(unsigned)* loc);

void __impl_mut_lock_slow(_Atomic(unsigned)* loc)
{
#ifdef BENCH
  size_t slow = 0;
  size_t futex = 0;
  size_t again = 0;
  size_t spin = 0;
#endif
  unsigned spins = 0;
  unsigned val = 1+atomic_fetch_add_explicit(loc, 1, memory_order_relaxed);
  if (!(val & lockbit)) goto BIT_UNSET;
  /* The lock acquisition loop. This has been designed such that the
     only possible change that is done inside that loop is setting
     the lock bit. This has a double objective. First all atomic
     operations are expensive and doing a pair of ++ and -- inside
     the loop would just waste memory bandwidth. Then, less changes
     to the count, means that other threads that are inside this
     same loop are less perturbed. */
  for (;;) {
    /* The lock bit is set by someone else, wait until it is
       unset. */
    for (spins = 0; spins < 10; ++spins) {
      a_spin();
      val = atomic_load_explicit(loc, memory_order_consume);
      if (!(val & lockbit)) goto BIT_UNSET;
    }
    do {
      a_spin();
      ACCOUNT(futex, 1);
      if (__syscall(SYS_futex, loc, FUTEX_WAIT|FUTEX_PRIVATE, val, 0) == -EAGAIN)
        ACCOUNT(again, 1);
      val = atomic_load_explicit(loc, memory_order_consume);
    } while (val & lockbit);
    /* The lock bit isn't set, try to acquire it. */
  BIT_UNSET:
    ACCOUNT(spin, spins);
    ACCOUNT(slow, 1);
    do {
      if (atomic_compare_exchange_strong_explicit(loc, &val, val|lockbit, memory_order_acq_rel, memory_order_consume)) {
#ifdef BENCH
        __impl_total += 1;
        __impl_slow += slow;
        __impl_futex += futex;
        __impl_again += again;
        __impl_spin += spin;
#endif
        return;
      }
      a_spin();
    } while (!(val & lockbit));
  }
}

void __impl_mut_unlock_slow(_Atomic(unsigned)* loc)
{
  __syscall(SYS_futex, loc, FUTEX_WAKE|FUTEX_PRIVATE, 1);
}
