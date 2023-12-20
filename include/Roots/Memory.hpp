#ifndef Roots_Memory_hpp
#define Roots_Memory_hpp

#include "./_defines.hpp"
#include "Debug.hpp"
#include <cstring>
#include <list>
#include <map>
#include <mutex>
#include <vector>
#include <chrono>

#define RootsMemBench_Start()                                                   \
  auto __start = std::chrono::high_resolution_clock::now()
#define RootsMemBench_End(v)                                                    \
  do {                                                                         \
    auto __end = std::chrono::high_resolution_clock::now();                    \
    auto __dur = std::chrono::duration_cast<std::chrono::microseconds>(        \
        __end - __start);                                                      \
    RootsDebugLog << "done in " << __dur.count() << "us";                       \
  } while (0);                                                                 \
  return v

namespace roots::mem {

struct __sys_align_t {
  u8 c;
  u64 sz;
};

static auto mult8RoundUp(const u64 size) -> const u64 {
  return (size + 7) & ~7;
}

/* Generic Allocator */

class Allocator {
protected:
  u64 _totalAlloc;
  u64 _totalAllocRequested;
  u64 _totalManuallyAlloc;

  std::mutex _memLock;

public:
  Allocator() = default;
  ~Allocator() {
#ifdef RootsDebug
    RootsDebugLog << "TA  " << _totalAlloc << " bytes"
                 << "TAR " << _totalAllocRequested << " bytes"
                 << "TMA " << _totalManuallyAlloc << " bytes";
#endif
  }

  virtual auto allocate(const u64 size, const u64 alignment = 0) -> void * = 0;
  virtual auto free(void *ptr, u64 size) -> void = 0;
};

/* Pool Allocator */

struct MemoryPool {
  u8 *head;
  u8 *mem;
};

class PoolAllocator : public Allocator {
  std::vector<MemoryPool> _pools;
  std::map<u64, std::list<u8 *>> _freeChunks;

  auto allocPool() -> void;

public:
  static constexpr u64 kPoolSize = 4 * 1024;
  static constexpr u64 kPoolAlignment = sizeof(__sys_align_t) - sizeof(u64);

  PoolAllocator() : Allocator() { allocPool(); }
  ~PoolAllocator() {
    for (auto &c : _freeChunks) {
      if (c.first > kPoolSize) {
        for (auto &blk : c.second) {
          delete[] blk;
        }
      }
      c.second.clear();
    }
    _freeChunks.clear();

    for (auto &p : _pools)
      delete[] p.mem;
  }

  auto allocate(const u64 size, const u64 alignment = 0) -> void * override;
  auto free(void *ptr, u64 size) -> void override;
};

static Allocator *allocator = new PoolAllocator();

static auto alloc(const u64 size, const u64 alignment = 0) -> void * {
  RootsMemBench_Start();
  RootsMemBench_End(allocator->allocate(size, alignment));
}

static auto free(void *ptr, u64 size) -> void {
  RootsMemBench_Start();
  RootsMemBench_End(allocator->free(ptr, size));
}

static auto zero(void *ptr, u64 size) -> void {
  RootsMemBench_Start();
  std::memset(ptr, 0, size);
  RootsMemBench_End();
}

template<typename T>
static auto alloc(const u64 count) -> T * {
  RootsMemBench_Start();
  RootsMemBench_End(static_cast<T *>(alloc(sizeof(T) * count)));
}

} // namespace roots::mem

#endif
