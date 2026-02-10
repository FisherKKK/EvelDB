#ifndef STORAGE_EVELDB_UTIL_HASH_H_
#define STORAGE_EVELDB_UTIL_HASH_H_

#include <cstddef>
#include <cstdint>
namespace eveldb {
uint32_t Hash(const char* data, size_t n, uint32_t seed);
}

#endif