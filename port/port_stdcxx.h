#ifndef STORAGE_EVELDB_PORT_PORT_STDCXX_H_
#define STORAGE_EVELDB_PORT_PORT_STDCXX_H_

#include <cassert>
#include <condition_variable>
#include <mutex>

#include "port/thread_annotations.h"
namespace eveldb {
namespace port {

class CondVar;

class LOCKABLE Mutex {
 public:
  Mutex() = default;
  ~Mutex() = default;

  Mutex(const Mutex&) = delete;
  Mutex& operator=(const Mutex&) = delete;

  void Lock() EXCLUSIVE_LOCK_FUNCTION() { mu_.lock(); }

  void Unlock() UNLOCK_FUNCTION() { mu_.unlock(); }

  void AssertHeld() ASSERT_EXCLUSIVE_LOCK() {}

 private:
  friend class CondVar;
  std::mutex mu_;
};

class CondVar {
 public:
  explicit CondVar(Mutex* mu) : mu_(mu) { assert(mu != nullptr); }
  ~CondVar() = default;

  CondVar(const CondVar&) = delete;
  CondVar& operator=(const CondVar&) = delete;

  void Wait() {
    // just manage the mutex (not lock)
    std::unique_lock<std::mutex> lock(mu_->mu_, std::adopt_lock);
    cv_.wait(lock);
    lock.release();
  }

  void Signal() { cv_.notify_one(); }
  void SingalAll() { cv_.notify_all(); }

 private:
  std::condition_variable cv_;
  Mutex* const mu_;
};

}  // namespace port
}  // namespace eveldb

#endif