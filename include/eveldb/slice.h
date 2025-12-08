#ifndef STORAGE_EVELDB_INCLUDE_SLICE_H_
#define STORAGE_EVELDB_INCLUDE_SLICE_H_

#include <cstddef>
#include <cstring>
#include <string>

#include "eveldb/export.h"

namespace eveldb {

// a reference of a string
class EVELDB_EXPORT Slice {
 public:
  // Create a empty slice
  Slice() : data_(""), size_(0) {}
  // Create a slice from d[0, n-1]
  Slice(const char* d, size_t n) : data_(d), size_(n) {}
  // Create a slice from s
  Slice(const std::string& s) : data_(s.data()), size_(s.size()) {}
  // Create a slice from s
  Slice(const char* s) : data_(s), size_(strlen(s)) {}

  size_t size() const { return size_; }
  const char* data() const { return data_; }

 private:
  const char* data_;
  size_t size_;
};
}  // namespace eveldb

#endif