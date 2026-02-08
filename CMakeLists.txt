cmake_minimum_required(VERSION 3.22)


# 1. project metadata
# project name, version, description, language
project(eveldb 
        VERSION 0.0.1
        DESCRIPTION "Minimal Leveldb" 
        LANGUAGES CXX)

# 2. compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # dont't use extention version of cxx17

# 3. project options
option(EVELDB_BUILD_EXAMPLES "Build example" OFF)
option(EVELDB_BUILD_TESTS "Build test cases" ON)

# 4. set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 4.1 inlcude extern project
include(extern/catch2/catch2.cmake)

# 5. define core library and source
set(EVELDB_PUBLIC_INCLUDE_DIR 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/eveldb")

add_library(eveldb "")
target_sources(eveldb
    PRIVATE
        "db/db_impl.cc"
        "db/db_impl.h"
        "db/skiplist.h"

        "util/status.cc"
        "util/arena.cc"
        "util/arena.h"
        "util/random.h"

    PUBLIC
        "${EVELDB_PUBLIC_INCLUDE_DIR}/slice.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/status.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/db.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/export.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/options.h"

)

# for test program
# make them find the dependencies
include_directories(
  "${PROJECT_BINARY_DIR}/include"
  "."
)

target_include_directories(eveldb
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    # PRIVATE
    #     "${PROJECT_BINARY_DIR}/include"
    #     "${PROJECT_SOURCE_DIR}/include/eveldb"
    #     "${PROJECT_SOURCE_DIR}"
)

# 5.1 Build test
if(EVELDB_BUILD_TESTS)
add_library(eveldb_tests "")
target_sources(eveldb_tests
    PRIVATE
    "util/status_test.cc"
    "util/arena_test.cc"
)
target_link_libraries(eveldb_tests Catch2::Catch2 eveldb)
endif(EVELDB_BUILD_TESTS)

# 5.2 add subdirectory
add_subdirectory(tests)
