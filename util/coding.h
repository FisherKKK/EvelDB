#ifndef STORAGE_EVELDB_UTIL_CODING_H_
#define STORAGE_EVELDB_UTIL_CODING_H_

#include <cstdint>
namespace eveldb {
inline uint32_t DecodeFixed32(const char* ptr) {
  const uint8_t* const buffer = reinterpret_cast<const uint8_t*>(ptr);
  return (static_cast<uint32_t>(buffer[0])) |
         (static_cast<uint32_t>(buffer[1]) << 8) |
         (static_cast<uint32_t>(buffer[2]) << 16) |
         (static_cast<uint32_t>(buffer[3]) << 24);
}
}  // namespace eveldb

#endif