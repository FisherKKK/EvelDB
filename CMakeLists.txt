# cmake_minimum_required(VERSION 3.9)
# project(eveldb LANGUAGES CXX)
# set(CMAKE_CXX_STANDARD 17)

# set(EVELDB_PUBLIC_INCLUDE_DIR "include/eveldb")
# include_directories("${PROJECT_BINARY_DIR}/include"
# ".")

# include(GNUInstallDirs)

# add_library(eveldb "")
# target_sources(eveldb
#     PRIVATE
#     "db/db_impl.cc"
#     "db/db_impl.h"
#     PUBLIC
#     "${EVELDB_PUBLIC_INCLUDE_DIR}/db.h"
#     "${EVELDB_PUBLIC_INCLUDE_DIR}/export.h"
#     "${EVELDB_PUBLIC_INCLUDE_DIR}/options.h"
#     "${EVELDB_PUBLIC_INCLUDE_DIR}/status.h"
# )

# target_include_directories(eveldb
#     PUBLIC
#     $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
#     $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
# )


# add_executable(test_interface "")
# target_sources(test_interface
#     PRIVATE
#     "tests/test_interface.cc"
# )
# target_link_libraries(test_interface eveldb)

cmake_minimum_required(VERSION 3.9)
project(eveldb LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 强制要求 C++17，不仅是建议

set(EVELDB_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/eveldb")

if (WIN32)
# do nothing now
else (WIN32)
    set(EVELDB_PLATFORM_NAME EVELDB_PLATFORM_POSIX) # 检查平台的相关信息
endif (WIN32)

add_library(eveldb "")
target_sources(eveldb
    PRIVATE
        "db/db_impl.cc"
        "db/db_impl.h"
        "db/filename.cc"
        "db/filename.h"
        "db/memtable.h"
        "db/version_edit.cc"
        "db/version_edit.h"

        "util/status.cc"
        "util/options.cc"
    PUBLIC
        "${EVELDB_PUBLIC_INCLUDE_DIR}/db.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/export.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/options.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/status.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/env.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/slice.h"
)

if (WIN32)
# do nothing
else (WIN32)
  target_sources(eveldb
    PRIVATE
      "util/env_posix.cc"
  )
endif (WIN32)

target_include_directories(eveldb
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        "${PROJECT_BINARY_DIR}/include"
        "${PROJECT_SOURCE_DIR}/include/eveldb" 
        "${PROJECT_SOURCE_DIR}"
)

target_compile_definitions(eveldb
    PRIVATE
    ${EVELDB_PLATFORM_NAME}=1
)

add_executable(test_interface "")
target_sources(test_interface
    PRIVATE
    "tests/test_interface.cc"
)


target_link_libraries(test_interface PRIVATE eveldb)