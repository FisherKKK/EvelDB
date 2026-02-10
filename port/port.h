#ifndef STORAGE_EVELDB_PORT_PORT_H_
#define STORAGE_EVELDB_PORT_PORT_H_

#if defined(EVELDB_PLATFORM_POSIX) || defined(EVELDB_PLATFORM_WINDOWS)
#include "port/port_stdcxx.h"
#endif

#endif