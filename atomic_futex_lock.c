#include "pthread_impl.h"
#include "stdatomic-impl.h"
#include <time.h>
#include <stdio.h>
#include <math.h>

/* The HO bit. */
static unsigned const lockbit = (UINT_MAX/2u)+1u;
static unsigned const contrib = (UINT_MAX/2u)+2u;

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

/* This is just a heuristic. We assume that one round of spinning is
   10 times faster than a failed call to futex_wait. */
static unsigned spins_max = 30;

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
    /* The lock bit is set by someone else, spin until it is unset. */
    for (spins = 0; spins < spins_max; ++spins) {
      a_spin();
      /* be optimistic and hope that the lock has been released */
      unsigned des = val-1;
      val -= contrib;
      if (atomic_compare_exchange_strong_explicit(loc, &val, des, memory_order_acq_rel, memory_order_consume))
        goto FINISH;
      if (!(val & lockbit)) goto BIT_UNSET;
    }
    /* The same inner loop as before, but with futex wait instead of
       a_spin. */
    for (;;) {
      ACCOUNT(futex, 1);
      if (__syscall(SYS_futex, loc, FUTEX_WAIT|FUTEX_PRIVATE, val, 0) == -EAGAIN)
        ACCOUNT(again, 1);
      /* be optimistic and hope that the lock has been released */
      unsigned des = val-1;
      val -= contrib;
      if (atomic_compare_exchange_strong_explicit(loc, &val, des, memory_order_acq_rel, memory_order_consume))
        goto FINISH;
      if (!(val & lockbit)) goto BIT_UNSET;
    }
    /* The lock bit isn't set, try to acquire it. */
  BIT_UNSET:
    ACCOUNT(spin, spins);
    ACCOUNT(slow, 1);
    do {
      a_spin();
      if (atomic_compare_exchange_strong_explicit(loc, &val, val|lockbit, memory_order_acq_rel, memory_order_consume))
        goto FINISH;
    } while (!(val & lockbit));
  }
 FINISH:
#ifdef BENCH
  __impl_total += 1;
  __impl_slow += slow;
  __impl_futex += futex;
  __impl_again += again;
  __impl_spin += spin;
#endif
  return;
}

void __impl_mut_unlock_slow(_Atomic(unsigned)* loc)
{
  __syscall(SYS_futex, loc, FUTEX_WAKE|FUTEX_PRIVATE, 1);
}

static
double timespecdiff(struct timespec* end, struct timespec* start) {
  double ret = end->tv_sec - start->tv_sec;
  if (end->tv_nsec >= start->tv_nsec)
    ret += (end->tv_nsec - start->tv_nsec)*1E-9;
  else
    ret += (start->tv_nsec - end->tv_nsec)*1E-9 - 1.0;
  /* fprintf(stderr, "bench start %lld %ld\n", start->tv_sec, start->tv_nsec); */
  /* fprintf(stderr, "bench end %lld %ld\n", end->tv_sec, end->tv_nsec); */
  /* fprintf(stderr, "difference %g\n", ret); */
  return ret;
}

void atomic_calibrate(void) {
  _Atomic(unsigned) loc = ATOMIC_VAR_INIT(42u);
  unsigned val = 0;
  size_t i;
  enum { iterI = 1000, };
  size_t j;
  enum { iterJ = 10, };
  double futex_time[iterJ] = { 0 };
  double spin_time[iterJ] = { 0 };
  struct timespec all_start, all_end, futex_start, futex_end, spin_start, spin_end;

  clock_gettime(CLOCK_MONOTONIC, &all_start);

  for (j = 0; j < iterJ; ++j) {
    /* Determine the cost of mis-predicted futex_wait */
    clock_gettime(CLOCK_MONOTONIC, &futex_start);
    for (i = 0; i < iterI; ++i) {
    SHORTCUT0:
      __syscall(SYS_futex, &loc, FUTEX_WAIT|FUTEX_PRIVATE, val, 0);
      /* Change the value in the same way as in the real loop. */
      unsigned des = val-1;
      val -= contrib;
      /* have the same conditionals as in the original, only that we
         know that this will never trigger */
      if (atomic_compare_exchange_strong_explicit(&loc, &val, des, memory_order_acq_rel, memory_order_consume))
        goto SHORTCUT0;
      /* Make sure that val never is correct. */
      val = 0;
    }
    clock_gettime(CLOCK_MONOTONIC, &futex_end);

    /* Determine the cost of mis-predicted futex_wait */
    clock_gettime(CLOCK_MONOTONIC, &spin_start);
    for (i = 0; i < iterI; ++i) {
    SHORTCUT1:
      a_spin();
      /* Change the value in the same way as in the real loop. */
      unsigned des = val-1;
      val -= contrib;
      /* have the same conditionals as in the original, only that we
         know that this will never trigger */
      if (atomic_compare_exchange_strong_explicit(&loc, &val, des, memory_order_acq_rel, memory_order_consume))
        goto SHORTCUT1;
      /* Make sure that val never is correct. */
      val = 0;
    }
    clock_gettime(CLOCK_MONOTONIC, &spin_end);

    futex_time[j] = timespecdiff(&futex_end, &futex_start)/iterI;
    spin_time[j] = timespecdiff(&spin_end, &spin_start)/iterI;
  }

  double ft1 = 0;
  double fmin= futex_time[0];
  double fmax= futex_time[0];
  double st1 = 0;
  double smin= spin_time[0];
  double smax= spin_time[0];

  for (j = 0; j < iterJ; ++j) {
    ft1 += futex_time[j];
    if (futex_time[j] < fmin) fmin = futex_time[j];
    if (fmax < futex_time[j]) fmax = futex_time[j];
    st1 += spin_time[j];
    if (spin_time[j] < smin) smin = spin_time[j];
    if (smax < spin_time[j]) smax = spin_time[j];
  }
  /* Throw away the extreme points of our measurements. */
  double ftm = (ft1-fmin-fmax)/(iterJ-2);
  double stm = (st1-smin-smax)/(iterJ-2);

  ft1 = 0;
  st1 = 0;

  double factor = 0.9;
  double estimate = ftm*factor/stm;
  if (estimate < 5.0) spins_max = 5u;
  else {
    if (100.0 < estimate) spins_max = 100u;
    else spins_max = estimate;
  }
  clock_gettime(CLOCK_MONOTONIC, &all_end);

  double ft2 = 0;
  double st2 = 0;

  for (j = 0; j < iterJ; ++j) {
    double fd1 = futex_time[j] - ftm;
    ft1 += fd1;
    ft2 += fd1*fd1;
    double sd1 = spin_time[j] - stm;
    st1 += sd1;
    st2 += sd1*sd1;
  }

  double ftd = sqrt((ft2 - (ft1*ft1)/iterJ)/(iterJ-1));
  double std = sqrt((st2 - (st1*st1)/iterJ)/(iterJ-1));

  fprintf(stderr, "end of calibration after %g sec:\n\tspin\t= %g (+%g)\n\tfail\t= %g (+%g)\n\tspins_max\t= %u\n\testimate\t= %g\n",
          timespecdiff(&all_end, &all_start), stm, std, ftm, ftd, spins_max, estimate);
}

#pragma weak atomic_inject
void atomic_inject(void) {
  /* empty */
}

_Thread_local _Bool atomic_faulty = 0;
