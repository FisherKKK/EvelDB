#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "eveldb/status.h"
#ifndef __Fuchsia__
#include <sys/resource.h>
#endif
#include <atomic>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <queue>
#include <set>
#include <string>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <thread>
#include <type_traits>
#include <unistd.h>
#include <utility>

#include "eveldb/env.h"

#include "port/port.h"

namespace eveldb {

namespace {
#if defined(HAVE_O_CLOEXEC)
constexpr const int kOpenBaseFlags = O_CLOEXEC;
#else
constexpr const int kOpenBaseFlags = 0;
#endif

Status PosixError(const std::string& context, int error_number) {
  if (error_number == ENOENT) {
    return Status::NotFound(context, std::strerror(error_number));
  } else {
    return Status::IOError(context, std::strerror(error_number));
  }
}

int LockOrUnlock(int fd, bool lock) {
  errno = 0;
  struct ::flock file_lock_info;
  std::memset(&file_lock_info, 0, sizeof(file_lock_info));
  file_lock_info.l_type = (lock ? F_WRLCK : F_ULOCK);
  file_lock_info.l_whence = SEEK_SET;
  file_lock_info.l_start = 0;
  file_lock_info.l_len = 0;  // Lock/unlock entire file
  return ::fcntl(fd, F_SETLK, &file_lock_info);
}

class PosixLockTable {
 public:
  bool Insert(const std::string& filename) {
    mu_.Lock();
    bool succeed = locked_files_.insert(filename).second;
    mu_.Unlock();
    return succeed;
  }

  void Remove(const std::string& filename) {
    mu_.Lock();
    locked_files_.erase(filename);
    mu_.Unlock();
  }

 private:
  port::Mutex mu_;
  std::set<std::string> locked_files_;
};

class PosixFileLock : public FileLock {
 public:
  PosixFileLock(int fd, std::string filename)
      : fd_(fd), filename_(std::move(filename)) {}

  int fd() const { return fd_; }
  const std::string& filename() const { return filename_; }

 private:
  const int fd_;
  const std::string filename_;
};

}  // namespace

class PosixEnv : public Env {
 public:
  Status CreateDir(const std::string& dirname) override {
    if (::mkdir(dirname.c_str(), 0755) != 0) {
      return PosixError(dirname, errno);
    }
    return Status::OK();
  }

  Status LockFile(const std::string& filename, FileLock** lock) override {
    *lock = nullptr;
    // 1. Create the lock file
    int fd = ::open(filename.c_str(), O_RDWR | O_CREAT | kOpenBaseFlags);
    if (fd < 0) {
      return PosixError(filename, errno);
    }

    // 2. Insert into the locktable in case of the same thread do the same thing
    // because lockfile only check the process level
    if (!locks_.Insert(filename)) {
      ::close(fd);
      return Status::IOError("lock " + filename, "already held by process");
    }

    // 3. Lock the file
    if (LockOrUnlock(fd, true) == -1) {
      int lock_errno = errno;
      ::close(fd);
      locks_.Remove(filename);
      return PosixError("lock " + filename, lock_errno);
    }

    *lock = new PosixFileLock(fd, filename);
    return Status::OK();
  }

  Status UnlockFile(FileLock* lock) override {
    PosixFileLock* posix_file_lock = static_cast<PosixFileLock*>(lock);
    if (LockOrUnlock(posix_file_lock->fd(), false) == -1) {
      return PosixError("unlock " + posix_file_lock->filename(), errno);
    }
    locks_.Remove(posix_file_lock->filename());
    ::close(posix_file_lock->fd());
    delete posix_file_lock;
    return Status::OK();
  }

 private:
  PosixLockTable locks_;
};

Env* Env::Default() {
  static PosixEnv env;
  return &env;
}

class PosixFileLock : public FileLock {};

}  // namespace eveldb