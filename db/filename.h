#ifndef STORAGE_EVELDB_DB_FILENAME_H_
#define STORAGE_EVELDB_DB_FILENAME_H_

#include <string>

namespace eveldb {
std::string LockFileName(const std::string& dbname);

};

#endif