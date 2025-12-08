#ifndef STORAGE_EVELDB_INCLUDE_OPTIONS_H_
#define STORAGE_EVELDB_INCLUDE_OPTIONS_H_

#include "eveldb/export.h"

namespace eveldb {
class Env;

struct EVELDB_EXPORT Options {
  Options();

  // Use the specified object to interact with the environment,
  // e.g. to read/write files, schedule background work, etc.
  // Default: Env::Default()
  Env* env;
};

}  // namespace eveldb

#endif