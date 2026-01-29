#ifndef STORAGE_EVELDB_INCLUDE_STATUS_H_
#define STORAGE_EVELDB_INCLUDE_STATUS_H_

#include <cstring>
#include <string>

#include "eveldb/export.h"
#include "eveldb/slice.h"

namespace eveldb {
class EVELDB_EXPORT Status {
 public:
  Status() noexcept : state_(nullptr) {}
  ~Status() { delete state_; }

  Status(const Status& rhs);
  Status& operator=(const Status& rhs);

  Status(Status&& rhs) noexcept : state_(rhs.state_) { rhs.state_ = nullptr; }
  Status& operator=(Status&& rhs) noexcept;

  static Status OK() { return Status(); }

  static Status NotFound(const Slice& msg, const Slice& mgs2 = Slice()) {
    return Status(kNotFound, msg, mgs2);
  }

  static Status Corruption(const Slice& msg, const Slice& mgs2 = Slice()) {
    return Status(kCorruption, msg, mgs2);
  }

  static Status NotSupported(const Slice& msg, const Slice& mgs2 = Slice()) {
    return Status(kNotSupported, msg, mgs2);
  };

  static Status InvalidArgument(const Slice& msg, const Slice& mgs2 = Slice()) {
    return Status(kInvalidArgument, msg, mgs2);
  };

  static Status IOError(const Slice& msg, const Slice& mgs2 = Slice()) {
    return Status(kIOError, msg, mgs2);
  };

  bool ok() const { return state_ == nullptr; }

  bool isNotFound() const { return code() == kNotFound; }

  bool isCorruption() const { return code() == kCorruption; }

  bool isNotSupportedError() const { return code() == kNotSupported; }

  bool isInvalidArgument() const { return code() == kInvalidArgument; }

  bool isIOError() const { return code() == kIOError; }

  std::string ToString() const;

 private:
  enum Code {
    kOk = 0,
    kNotFound = 1,
    kCorruption = 2,
    kNotSupported = 3,
    kInvalidArgument = 4,
    kIOError = 5
  };

  // msg: msg2
  Status(Code code, const Slice& msg, const Slice& msg2);

  static const char* CopyState(const char* s);
  Code code() const {
    return (state_ == nullptr) ? kOk : static_cast<Code>(state_[4]);
  }

  // state == nullptr -> OK
  // state != nullptr -> [0..3] = length of message, [4] = code, [5..] = message
  const char* state_;
};

inline Status::Status(const Status& rhs) {
  state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
}

inline Status& Status::operator=(const Status& rhs) {
  if (state_ != rhs.state_) {
    delete[] state_;
    state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
  }
  return *this;
}

inline Status& Status::operator=(Status&& rhs) noexcept {
  std::swap(rhs.state_, state_);
  return *this;
}

}  // namespace eveldb

#endif