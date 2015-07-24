
#include <stdatomic.h>

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16
INSTANTIATE_STUB_LF(16, __uint128_t)
#else
INSTANTIATE_STUB_LC(16, __uint128_t)
#endif
