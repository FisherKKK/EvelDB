#ifndef STORAGE_EVELDB_DB_SKIPLIST_H_
#define STORAGE_EVELDB_DB_SKIPLIST_H_
#include "util/arena.h"

namespace eveldb {

template <typename Key, typename Comparator>
class SkipList {
 private:
  struct Node;

 public:
  explicit SkipList(Comparator cmp, Arena* arena);
};

template <typename Key, typename Comparator>
struct SkipList<Key, Comparator>::Node {};

}  // namespace eveldb

#endif