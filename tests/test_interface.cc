#include "eveldb/db.h"

int main() {
  eveldb::DB* db;

  auto status = eveldb::DB::Open(eveldb::Options(), "/tmp/test-0", &db);
  return 0;
}