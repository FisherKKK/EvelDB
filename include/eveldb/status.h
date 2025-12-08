#ifndef STORAGE_EVELDB_INCLUDE_STATUS_H_
#define STORAGE_EVELDB_INCLUDE_STATUS_H_

#include <string>

#include "eveldb/export.h"
#include "eveldb/slice.h"

namespace eveldb {

class EVELDB_EXPORT Status {
 public:
  Status() noexcept : state_(nullptr) {}
  ~Status() { delete[] state_; }

  Status(const Status& rhs);
  Status& operator=(const Status& rhs);

  Status(Status&& rhs) noexcept : state_(rhs.state_) { rhs.state_ = nullptr; }
  Status& operator=(Status&& rhs) noexcept;

  static Status OK() { return Status(); }
  static Status NotFound(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(Code::kNotFound, msg, msg2);
  }
  static Status IOError(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(Code::kIOError, msg, msg2);
  }

  bool ok() const { return state_ == nullptr; }

 private:
  enum class Code {
    kOk = 0,
    kNotFound = 1,
    kCorruption = 2,
    kNotSupported = 3,
    kInvalidArgument = 4,
    kIOError = 5
  };

  Status(Code code, const Slice& msg, const Slice& msg2);
  static const char* CopyState(const char* s);
  // OK status has a null state_. Otherwise, state_ is a new[] array
  // of the following form:
  // state_[0..3] == length of message
  // state_[4]    == code
  // state_[5..]    == message
  const char* state_;
};

inline Status::Status(const Status& rhs) {
  state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
}

inline Status& Status::operator=(const Status& rhs) {
  // Must check itself
  if (state_ != rhs.state_) {
    delete[] state_;
    state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
  }
  return *this;
}

inline Status& Status::operator=(Status&& rhs) noexcept {
  // Elegant dealing method
  std::swap(state_, rhs.state_);
  return *this;
}

}  // namespace eveldb

#endif