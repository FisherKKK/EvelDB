#ifndef STORAGE_EVELDB_INCLUDE_ENV_H_
#define STORAGE_EVELDB_INCLUDE_ENV_H_

#include <string>

#include "eveldb/export.h"
#include "eveldb/status.h"

namespace eveldb {
class FileLock;

class EVELDB_EXPORT Env {
 public:
  static Env* Default();

  // Create the specified directory
  virtual Status CreateDir(const std::string& dirname) = 0;

  virtual Status LockFile(const std::string& fname, FileLock** lock) = 0;

  virtual Status UnlockFile(FileLock* lock) = 0;
};

class EVELDB_EXPORT FileLock {};

}  // namespace eveldb

#endif