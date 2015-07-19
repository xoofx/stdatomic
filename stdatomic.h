#ifndef _STDATOMIC_H_
#define _STDATOMIC_H_ 1

#include "atomic_constants.h"
#include "atomic_flag.h"
#include "atomic_lock.h"
#include "atomic_fence.h"
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* In some places we need a type that is almost the same as base type
   T, but

   - returns a pointer to T in evaluation context
   - can't be assigned to

   T can be a type or an expression.
*/
#define __atyp(T) __typeof__(__typeof__(T)[1])

/* To evaluate expressions we sometimes need temporaries of that type
   with a certain value. */
#define __atmp(T, V) (__atyp(T)){ [0] = (V), }

/* When evaluating lvalues in gcc's compound expressions to return a
   value, we want to take care that these lvalues can't be
   accidentally be subject to the & operator. Force it to be an
   rvalue. */
#define __aret(V) (1 ? (V) : (V))


#ifdef __ATOMIC_FORCE_SYNC
#include "stdatomic-gcc-sync.h"
#elif defined(__clang__)
#include "stdatomic-clang-c11.h"
#else
#include "stdatomic-gcc-atomic.h"
#endif

#include "atomic_types.h"

#endif
