#ifndef STORAGE_EVELDB_INCLUDE_ENV_H_
#define STORAGE_EVELDB_INCLUDE_ENV_H_

#include "eveldb/export.h"
#include "eveldb/status.h"

namespace eveldb {

class EVELDB_EXPORT Env {
 public:
  Env();
  Env(const Env&) = delete;
  Env& operator=(const Env&) = delete;

  virtual ~Env();

  static Env* Default();

  virtual void Schedule(void (*function)(void* arg), void* arg) = 0;
};

}  // namespace eveldb

#endif