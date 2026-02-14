#ifndef STORAGE_EVELDB_INCLUDE_COMPARATOR_H_
#define STORAGE_EVELDB_INCLUDE_COMPARATOR_H_

#include "eveldb/export.h"
#include "eveldb/slice.h"
namespace eveldb {

class EVELDB_EXPORT Comparator {
 public:
  virtual ~Comparator();

  // Three-way comparision. Return value:
  // < 0 iff "a" < "b"
  // = 0 iff "a" = "b"
  // > 0 iff "a" > "b"
  virtual int Compare(const Slice& a, const Slice& b) const = 0;

  virtual const char* Name() const = 0;

  virtual void FindShortestSeparator(std::string* start,
                                     const Slice& limit) const = 0;

  virtual void FindShortSuccessor(std::string* key) const = 0;
};
}  // namespace eveldb

#endif