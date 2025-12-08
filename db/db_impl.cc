#include "db/db_impl.h"

#include "db/filename.h"
#include "db/version_edit.h"
#include <iostream>

namespace eveldb {

Status DB::Open(const Options& options, const std::string& dbname, DB** dbptr) {
  // 1. clear and new the dbimpl
  *dbptr = nullptr;
  DBImpl* impl = new DBImpl(options, dbname);
  // 2. lock the state and recover the db
  impl->mutex_.Lock();
  VersionEdit edit;
  bool save_manifest = false;
  Status s = impl->Recover(&edit, &save_manifest);
  if (s.ok() && impl->mem_ == nullptr) {
  }

  if (s.ok() && save_manifest) {
  }

  if (s.ok()) {
  }

  impl->mutex_.Unlock();
  if (s.ok()) {
  } else {
    delete impl;
  }

  delete impl;
  return s;
}

DB::~DB() = default;

DBImpl::DBImpl(const Options& raw_options, const std::string& dbname)
    : env_(raw_options.env), dbname_(dbname) {}

DBImpl::~DBImpl() {
  if (db_lock_ != nullptr) {
    env_->UnlockFile(db_lock_);
  }
}

Status DBImpl::Recover(VersionEdit* edit, bool* save_manifest) {
  // 1. judge the lock is still holding
  mutex_.AssertHeld();
  // 2. create db dir
  env_->CreateDir(dbname_);
  // 3. lock file
  Status s = env_->LockFile(LockFileName(dbname_), &db_lock_);
  // 4. TODO

  return s;
}

}  // namespace eveldb