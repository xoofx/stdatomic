#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "atomic_fence.h"
#include "atomic_lock.h"

enum { len = 1024 };

static atomic_lock table[len];

// Hash function found by Thomas Mueller in an answer to SO question
// https://stackoverflow.com/questions/664014
static
uint32_t hash32(uint32_t x) {
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x);
  return x;
}

#if UINTPTR_MAX == UINT32_MAX
static
size_t hash(void* p) {
  return hash32((uintptr_t)p)%len;
}
#else
static
size_t hash(void* p) {
  uint64_t bits = (uintptr_t)p;
  return (hash32(bits) ^ hash32(bits >> 32)) % len;
}
#endif


void atomic_load_internal (size_t size, void* ptr, void* ret, int mo) {
  size_t pos = hash(ptr);
  atomic_lock_lock(table+pos);
  if (mo == memory_order_seq_cst)
    atomic_thread_fence(memory_order_seq_cst);
  memcpy(ret, ptr, size);
  atomic_lock_unlock(table+pos);
}

void atomic_store_internal (size_t size, void* ptr, void* val, int mo) {
  size_t pos = hash(ptr);
  atomic_lock_lock(table+pos);
  memcpy(ptr, val, size);
  if (mo == memory_order_seq_cst)
    atomic_thread_fence(memory_order_seq_cst);
  atomic_lock_unlock(table+pos);
}

void atomic_exchange_internal (size_t size, void* ptr, void* val, void* ret, int mo) {
  size_t pos = hash(ptr);
  void* backup = (val == ret) ? malloc(size) : ret;
  atomic_lock_lock(table+pos);
  memcpy(backup, ptr, size);
  if (mo == memory_order_seq_cst)
    atomic_thread_fence(memory_order_seq_cst);
  memcpy(ptr, val, size);
  atomic_lock_unlock(table+pos);
  if (val == ret) {
    memcpy(ret, backup, size);
    free(backup);
  }
}

_Bool atomic_compare_exchange_internal (size_t size, void* ptr, void* expected, void* desired, _Bool weak, int mos, int mof) {
  size_t pos = hash(ptr);
  atomic_lock_lock(table+pos);
  _Bool ret = !memcmp(ptr, expected, size);
  if (ret) {
    memcpy(ptr, desired, size);
    if (mos == memory_order_seq_cst)
      atomic_thread_fence(memory_order_seq_cst);
  } else {
    if (mof == memory_order_seq_cst)
      atomic_thread_fence(memory_order_seq_cst);
    memcpy(expected, ptr, size);
  }
  atomic_lock_unlock(table+pos);
  return ret;
}
