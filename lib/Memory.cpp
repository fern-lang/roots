#include "Roots/Memory.hpp"

namespace roots::mem {

/* PoolAllocator */

auto PoolAllocator::allocPool() -> void {
  u8 *alloc = new u8[kPoolSize];
#ifdef RootsDebug
  _totalAlloc += kPoolSize;
#endif
  _pools.push_back({alloc, alloc});
}

auto PoolAllocator::allocate(u64 size, u64 alignment) -> void * {
  if (size == 0)
    return nullptr;

  std::lock_guard<std::mutex> lock(_memLock);

#ifdef RootsDebug
  ++_totalAllocRequested;
#endif

  size = mem::mult8RoundUp(size);

  if (size > kPoolSize) {
#ifdef RootsDebug
    RootsDebugLog << "Allocating manually ... " << size << " bytes" << std::endl;
    _totalManuallyAlloc += size;
#endif
    return new u8[size];
  }

  if (_freeChunks[size].size() == 0) {
    for (auto &p : _pools) {
      u64 freeSpace = kPoolSize - (p.head - p.mem);
      if (freeSpace >= size) {
        u8 *ret = p.head;
        p.head += size;
#ifdef RootsDebug
        RootsDebugLog << "Allocating from pool ... " << size << " bytes"
                     << std::endl;
#endif
        return ret;
      }
    }

    allocPool();

    auto &p = _pools.back();
    u8 *loc = p.head;
    p.head += size;
#ifdef RootsDebug
    RootsDebugLog << "Allocating from NEW pool ... " << size << " bytes"
                 << std::endl;
#endif
    return loc;
  }

  u8 *ret = _freeChunks[size].front();
  _freeChunks[size].pop_front();
#ifdef RootsDebug
  RootsDebugLog << "Using previously allocated ... " << size << " bytes"
               << std::endl;
#endif
  return ret;
}

auto PoolAllocator::free(void *ptr, u64 size) -> void {
  if (ptr == nullptr || size == 0)
    return;

  std::lock_guard<std::mutex> lock(_memLock);

  if (size > kPoolSize) {
#ifdef RootsDebug
    RootsDebugLog << "Freeing manually ... " << size << " bytes" << std::endl;
#endif
    delete[] (u8 *)ptr;
    return;
  }

#ifdef RootsDebug
  RootsDebugLog << "Freeing to pool ... " << size << " bytes" << std::endl;
#endif
  _freeChunks[size].push_back((u8 *)ptr);
}

} // namespace roots::mem
