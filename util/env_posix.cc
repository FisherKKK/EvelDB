#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <thread>
#include <type_traits>

#include "eveldb/env.h"

#include "port/port.h"
#include "port/thread_annotations.h"

namespace eveldb {

namespace {

class PosixEnv : public Env {
 public:
  PosixEnv();
  ~PosixEnv() override {
    static const char msg[] =
        "PosixEnv singleton destroyed. Unsupported behavior!\n";
    std::fwrite(msg, 1, sizeof(msg), stderr);
    std::abort();
  }

  void Schedule(void (*background_work_function)(void* background_work_arg),
                void* background_work_arg) override;

 private:
  void BackgroundThreadMain();

  static void BackgroundThreadEntryPoint(PosixEnv* env) {
    env->BackgroundThreadMain();
  }

  struct BackgroundWorkItem {
    explicit BackgroundWorkItem(void (*function)(void* arg), void* arg)
        : function(function), arg(arg) {}

    void (*const function)(void*);
    void* const arg;
  };

  port::Mutex background_work_mutex_;
  port::CondVar background_work_cv_ GUARDED_BY(background_work_mutex_);
  bool started_background_thread_ GUARDED_BY(background_work_mutex_);

  std::queue<BackgroundWorkItem> background_work_queue_
      GUARDED_BY(background_work_mutex_);
};

}  // namespace

PosixEnv::PosixEnv()
    : background_work_cv_(&background_work_mutex_),
      started_background_thread_(false) {}

void PosixEnv::Schedule(
    void (*background_work_function)(void* background_work_arg),
    void* background_work_arg) {
  background_work_mutex_.Lock();

  if (!started_background_thread_) {
    started_background_thread_ = true;
    std::thread background_thread(PosixEnv::BackgroundThreadEntryPoint, this);
    background_thread.detach();
  }

  if (background_work_queue_.empty()) {
    background_work_cv_.Signal();
  }

  background_work_queue_.emplace(background_work_function, background_work_arg);
  background_work_mutex_.Unlock();
}

void PosixEnv::BackgroundThreadMain() {
  while (true) {
    background_work_mutex_.Lock();

    while (background_work_queue_.empty()) {
      background_work_cv_.Wait();
    }

    assert(!background_work_queue_.empty());
    auto background_work_function = background_work_queue_.front().function;
    void* background_work_arg = background_work_queue_.front().arg;
    background_work_queue_.pop();

    background_work_mutex_.Unlock();
    background_work_function(background_work_arg);
  }
}

namespace {
template <typename EnvType>
class SingletonEnv {
 public:
  SingletonEnv() {
#if !defined(NDEBUG)
    env_initialized_.store(true, std::memory_order_relaxed);
#endif
    static_assert(sizeof(env_storage_) >= sizeof(EnvType),
                  "env_storage_ will not hold the Env");
    static_assert(std::is_standard_layout_v<SingletonEnv<EnvType>>);
    static_assert(
        offsetof(SingletonEnv<EnvType>, env_storage_) % alignof(EnvType) == 0,
        "env_storage_ does not meet the Env's alignment needs");
    static_assert(alignof(SingletonEnv<EnvType>) % alignof(EnvType) == 0,
                  "env_storage_ does not meet the Env's alignment need");
    new (env_storage_) EnvType();
  }

  ~SingletonEnv() = default;
  SingletonEnv(const SingletonEnv&) = delete;
  SingletonEnv& operator=(const SingletonEnv&) = delete;

  Env* env() { return reinterpret_cast<Env*>(&env_storage_); }

  static void AssertEnvNotInitialized() {
#if !defined(NDEBUG)
    assert(!env_initialized_.load(std::memory_order_relaxed));
#endif
  }

 private:
  alignas(EnvType) char env_storage_[sizeof(EnvType)];

#if !defined(NDEBUG)
  static std::atomic<bool> env_initialized_;
#endif
};

// C++17 can define in the class by inline
#if !defined(NDEBUG)
template <typename EnvType>
std::atomic<bool> SingletonEnv<EnvType>::env_initialized_;
#endif

using PosixDefaultEnv = SingletonEnv<PosixEnv>;
};  // namespace

Env* Env::Default() {
  static PosixDefaultEnv env_container;
  return env_container.env();
}
}  // namespace eveldb