#include "eveldb/options.h"

#include "eveldb/env.h"

namespace eveldb {
Options::Options() : env(Env::Default()) {}
}  // namespace eveldb