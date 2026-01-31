#ifndef STORAGE_EVELDB_UTIL_ARENA_H_
#define STORAGE_EVELDB_UTIL_ARENA_H_
#include <atomic>
#include <cstddef>
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
        return 0;
    }

private:
    char* AllocateFallback(size_t bytes);
    char* AllocateNewBlock(size_t block_bytes);

    // Allocation state

    // Total memory usage
    std::atomic<size_t> memory_usage_;
};
}  // namespace eveldb

#endif