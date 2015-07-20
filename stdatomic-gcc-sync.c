
#define __ATOMIC_FORCE_SYNC 1

#include "stdatomic.h"


INSTANTIATE_CAS(1, uint8_t);
INSTANTIATE_CAS(2, uint16_t);
INSTANTIATE_CAS(4, uint32_t);
INSTANTIATE_CAS(8, uint64_t);
INSTANTIATE_CAS(16, __uint128_t);
