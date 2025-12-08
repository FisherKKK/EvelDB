#ifndef STORAGE_EVELDB_DB_DB_IMPL_H_
#define STORAGE_EVELDB_DB_DB_IMPL_H_
#include "eveldb/db.h"
#include "eveldb/env.h"

#include "port/port.h"

namespace eveldb {

class MemTable;
class VersionEdit;

class DBImpl : public DB {
 public:
  DBImpl(const Options& raw_options, const std::string& dbname);

  ~DBImpl() override;

  Status Recover(VersionEdit* edit, bool* save_manifest);

 private:
  friend class DB;

  // Constant after construction
  Env* const env_;
  const std::string dbname_;

  // Lock over the persistent DB state. None-null iff successfully acquired
  FileLock* db_lock_;

  // State below is protected by mutex_
  port::Mutex mutex_;
  MemTable* mem_;
};

}  // namespace eveldb

#endif