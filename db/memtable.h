#ifndef STORAGE_EVELDB_DB_MEMTABLE_H_
#define STORAGE_EVELDB_DB_MEMTABLE_H_

#include "db/dbformat.h"
#include "db/skiplist.h"

#include "eveldb/iterator.h"
#include "eveldb/slice.h"
#include "eveldb/status.h"

#include "util/arena.h"
namespace eveldb {
class InternalKeyComparator;

class MemTable {
 public:
  explicit MemTable(const InternalKeyComparator& comparator);

  MemTable(const MemTable&) = delete;
  MemTable& operator=(const MemTable&) = delete;

  void Ref() { ++refs_; }

  void Unref() {
    --refs_;
    assert(refs_ >= 0);
    if (refs_ <= 0) {
      delete this;
    }
  }

  size_t ApproximateMemoryUsage();

  Iterator* NewIterator();

  void Add(SequenceNumber seq, ValueType type, const Slice& key,
           const Slice& value);

  bool Get(const LookupKey& key, std::string* value, Status* s);

 private:
  struct KeyComparator {
    const InternalKeyComparator comparator;
    explicit KeyComparator(const InternalKeyComparator& c) : comparator(c) {}
    int operator()(const char* a, const char* b) const;
  };

  typedef SkipList<const char*, KeyComparator> Table;

  ~MemTable();  // private since only Unref() should be used to delete it

  KeyComparator comparator_;
  int refs_;
  Arena arena_;
  Table table_;
};

}  // namespace eveldb

#endif