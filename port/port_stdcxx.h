#ifndef STORAGE_EVELDB_PORT_PORT_STDCXX_H_
#define STORAGE_EVELDB_PORT_PORT_STDCXX_H_

#include <cassert>
#include <mutex>

#include "port/thread_annotations.h"

namespace eveldb {
namespace port {

class LOCKABLE Mutex {
 public:
  void Lock() { mu_.lock(); }
  void Unlock() { mu_.unlock(); }
  void AssertHeld() {}

 private:
  std::mutex mu_;
};
}  // namespace port
}  // namespace eveldb
#endif