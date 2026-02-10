#ifndef STORAGE_EVELDB_UTIL_ARENA_H_
#define STORAGE_EVELDB_UTIL_ARENA_H_
#include <atomic>
#include <cassert>
#include <cstddef>
#include <vector>
namespace eveldb {
class Arena {
 public:
  Arena();

  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;

  ~Arena();

  char* Allocate(size_t bytes);

  char* AllocateAligned(size_t bytes);

  size_t MemoryUsage() const {
    return memory_usage_.load(std::memory_order_relaxed);
  }

 private:
  // Fallback allocate when direct allocation falls
  char* AllocateFallback(size_t bytes);
  // Allocate new block
  char* AllocateNewBlock(size_t block_bytes);

  // Allocation state
  char* alloc_ptr_;
  size_t alloc_bytes_remaining_;

  std::vector<char*> blocks_;

  // Total memory usage
  std::atomic<size_t> memory_usage_;
};

inline char* Arena::Allocate(size_t bytes) {
  assert(bytes > 0);
  if (alloc_bytes_remaining_ >= bytes) {
    char* result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
  }
  return AllocateFallback(bytes);
}
}  // namespace eveldb

#endif