#ifndef STORAGE_EVELDB_UTIL_TESTUTIL_H_
#define STORAGE_EVELDB_UTIL_TESTUTIL_H_

#include <catch2/catch_get_random_seed.hpp>

namespace eveldb {
namespace test {

inline int RandomSeed() { return Catch::getSeed(); }

}  // namespace test
}  // namespace eveldb

#endif