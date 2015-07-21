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

#ifndef __GCC_ATOMIC_BOOL_LOCK_FREE
#define __GCC_ATOMIC_BOOL_LOCK_FREE 2
#define __GCC_ATOMIC_CHAR_LOCK_FREE 2
#define __GCC_ATOMIC_SHORT_T_LOCK_FREE 2
# if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_16)
# define __GCC_ATOMIC_INT_T_LOCK_FREE 2
# define __GCC_ATOMIC_LONG_T_LOCK_FREE 2
# define __GCC_ATOMIC_LLONG_T_LOCK_FREE 2
# define __GCC_ATOMIC_POINTER_T_LOCK_FREE 2
# define __GCC_ATOMIC_CHAR16_T_LOCK_FREE 2
# define __GCC_ATOMIC_CHAR32_T_LOCK_FREE 2
# define __GCC_ATOMIC_WCHAR_T_LOCK_FREE 2
# elsif defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8)
# define __GCC_ATOMIC_INT_T_LOCK_FREE ((UINT_MAX <= 0xFFFFFFFFFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_LONG_T_LOCK_FREE ((ULONG_MAX <= 0xFFFFFFFFFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_LLONG_T_LOCK_FREE ((ULLONG_MAX <= 0xFFFFFFFFFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_POINTER_T_LOCK_FREE ((UINTPTR_MAX <= 0xFFFFFFFFFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_CHAR16_T_LOCK_FREE ((UINT_LEAST16_MAX <= 0xFFFFFFFFFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_CHAR32_T_LOCK_FREE ((UINT_LEAST32_MAX <= 0xFFFFFFFFFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_WCHAR_T_LOCK_FREE ((WCHAR_MAX <= 0xFFFFFFFFFFFFFFFFU) ? 2 : 0)
# elsif defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4)
# define __GCC_ATOMIC_INT_T_LOCK_FREE ((UINT_MAX <= 0xFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_LONG_T_LOCK_FREE ((ULONG_MAX <= 0xFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_LLONG_T_LOCK_FREE ((ULLONG_MAX <= 0xFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_POINTER_T_LOCK_FREE ((UINTPTR_MAX <= 0xFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_CHAR16_T_LOCK_FREE ((UINT_LEAST16_MAX <= 0xFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_CHAR32_T_LOCK_FREE ((UINT_LEAST32_MAX <= 0xFFFFFFFFU) ? 2 : 0)
# define __GCC_ATOMIC_WCHAR_T_LOCK_FREE ((WCHAR_MAX <= 0xFFFFFFFFU) ? 2 : 0)
# endif
#endif


#define ATOMIC_BOOL_LOCK_FREE       __GCC_ATOMIC_BOOL_LOCK_FREE
#define ATOMIC_CHAR_LOCK_FREE       __GCC_ATOMIC_CHAR_LOCK_FREE
#define ATOMIC_SHORT_T_LOCK_FREE    __GCC_ATOMIC_SHORT_T_LOCK_FREE
#define ATOMIC_INT_T_LOCK_FREE      __GCC_ATOMIC_INT_T_LOCK_FREE
#define ATOMIC_LONG_T_LOCK_FREE     __GCC_ATOMIC_LONG_T_LOCK_FREE
#define ATOMIC_LLONG_T_LOCK_FREE    __GCC_ATOMIC_LLONG_T_LOCK_FREE

#define ATOMIC_POINTER_T_LOCK_FREE  __GCC_ATOMIC_POINTER_T_LOCK_FREE

#define ATOMIC_CHAR16_T_LOCK_FREE   __GCC_ATOMIC_CHAR16_T_LOCK_FREE
#define ATOMIC_CHAR32_T_LOCK_FREE   __GCC_ATOMIC_CHAR32_T_LOCK_FREE
#define ATOMIC_WCHAR_T_LOCK_FREE    __GCC_ATOMIC_WCHAR_T_LOCK_FREE

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1
# define ATOMIC_UINT8_LOCK_FREE 2
#else
# define ATOMIC_UINT8_LOCK_FREE 0
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2
# define ATOMIC_UINT16_LOCK_FREE 2
#else
# define ATOMIC_UINT16_LOCK_FREE 0
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
# define ATOMIC_UINT32_LOCK_FREE 2
#else
# define ATOMIC_UINT32_LOCK_FREE 0
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
# define ATOMIC_UINT64_LOCK_FREE 2
#else
# define ATOMIC_UINT64_LOCK_FREE 0
#endif

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16
# define ATOMIC_UINT128_LOCK_FREE 2
#else
# define ATOMIC_UINT128_LOCK_FREE 0
#endif


#define atomic_is_lock_free(O)                                  \
(sizeof*(O) == 1 ? ATOMIC_UINT8_LOCK_FREE                       \
 : (sizeof*(O) == 2 ? ATOMIC_UINT16_LOCK_FREE                   \
    : (sizeof*(O) == 4 ? ATOMIC_UINT32_LOCK_FREE                \
       : ((sizeof*(O) == 8) ? ATOMIC_UINT64_LOCK_FREE           \
          : ((sizeof*(O) == 16) ? ATOMIC_UINT128_LOCK_FREE      \
             : 0)))))


#endif
