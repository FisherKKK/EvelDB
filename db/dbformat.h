#ifndef STORAGE_EVELDB_DB_DBFORMAT_H_
#define STORAGE_EVELDB_DB_DBFORMAT_H_
#include <cstdint>

#include "eveldb/comparator.h"
#include "eveldb/slice.h"

namespace eveldb {
class InternalKey;

enum ValueType { kTypeDeletion = 0x0, kTypeValue = 0x1 };
static constexpr ValueType kValueTypeForSeek = kTypeValue;

typedef uint64_t SequenceNumber;

static constexpr SequenceNumber kMaxSequenceNumber = ((0x1ull << 56) - 1);

inline Slice ExtractUserKey(const Slice& internal_key) {
  assert(internal_key.size() >= 8);
  return Slice(internal_key.data(), internal_key.size() - 8);
}

// Comparator for internal key, sort by key increase and seq num decrease
class InternalKeyComparator : public Comparator {
 private:
  const Comparator* user_comparator_;

 public:
  explicit InternalKeyComparator(const Comparator* c) : user_comparator_(c) {}
  const char* Name() const override;
  // sort by user key and seq number
  int Compare(const Slice& a, const Slice& b) const override;
  virtual void FindShortestSeparator(std::string* start,
                                     const Slice& limit) const override;

  virtual void FindShortSuccessor(std::string* key) const override;

  const Comparator* user_comparator() const { return user_comparator_; }
};

class LookupKey {
 public:
  LookupKey(const Slice& user_key, SequenceNumber sequence);

  LookupKey(const LookupKey&) = delete;
  LookupKey& operator=(const LookupKey&) = delete;

  Slice memtable_key() const { return Slice(start_, end_ - start_); }

  Slice internal_key() const { return Slice(kstart_, end_ - kstart_); }

  Slice user_key() const { return Slice(kstart_, end_ - 8 - kstart_); }

  ~LookupKey();

 private:
  // form:
  // klength  varint32         <-- start_
  // userkey  char[klength]    <-- kstart_
  // tag      uint64
  //                           <-- end
  const char* start_;
  const char* kstart_;
  const char* end_;
  char space_[200];
};

inline LookupKey::~LookupKey() {
  if (start_ != space_) delete[] start_;
}

}  // namespace eveldb

#endif