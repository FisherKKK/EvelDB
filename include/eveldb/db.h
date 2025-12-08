#ifndef STORAGE_EVELDB_INCLUDE_DB_H_
#define STORAGE_EVELDB_INCLUDE_DB_H_

#include "eveldb/export.h"
#include "eveldb/options.h"
#include "eveldb/status.h"

namespace eveldb {

class EVELDB_EXPORT DB {
 public:
  static Status Open(const Options& options, const std::string& name,
                     DB** dbptr);

  virtual ~DB();
};

}  // namespace eveldb

#endif
