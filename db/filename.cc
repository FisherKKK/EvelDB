#include "db/filename.h"

namespace eveldb {
std::string LockFileName(const std::string& dbname) { return dbname + "/LOCK"; }
}  // namespace eveldb