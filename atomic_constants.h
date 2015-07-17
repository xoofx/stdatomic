#ifndef _STDATOMIC_ATOMIC_CONSTANTS_H_
#define _STDATOMIC_ATOMIC_CONSTANTS_H_ 1

#ifndef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1
# error "this implementation of stdatomic need support that is compatible with the gcc ABI"
#endif

#if !defined(__ATOMIC_RELAXED) && !defined(__ATOMIC_FORCE_SYNC)
#define __ATOMIC_FORCE_SYNC 1
#endif

#ifndef __ATOMIC_RELAXED
#define __ATOMIC_RELAXED 0
#endif
#ifndef __ATOMIC_CONSUME
#define __ATOMIC_CONSUME 1
#endif
#ifndef __ATOMIC_ACQUIRE
#define __ATOMIC_ACQUIRE 2
#endif
#ifndef __ATOMIC_RELEASE
#define __ATOMIC_RELEASE 3
#endif
#ifndef __ATOMIC_ACQ_REL
#define __ATOMIC_ACQ_REL 4
#endif
#ifndef __ATOMIC_SEQ_CST
#define __ATOMIC_SEQ_CST 5
#endif

enum memory_order {
  memory_order_relaxed = __ATOMIC_RELAXED,
  memory_order_consume = __ATOMIC_CONSUME,
  memory_order_acquire = __ATOMIC_ACQUIRE,
  memory_order_release = __ATOMIC_RELEASE,
  memory_order_acq_rel = __ATOMIC_ACQ_REL,
  memory_order_seq_cst = __ATOMIC_SEQ_CST,
};
typedef enum memory_order memory_order;


#endif
