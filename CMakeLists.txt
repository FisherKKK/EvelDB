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

# 3. project options
option(BUILD_EXAMPLES "Build example" OFF)

# 4. set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 5. define core library and source
set(EVELDB_PUBLIC_INCLUDE_DIR 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/eveldb")

add_library(eveldb "")
target_sources(eveldb
    PRIVATE
        "db/db_impl.cc"
        "db/db_impl.h"

        "util/status.cc"

    PUBLIC
        "${EVELDB_PUBLIC_INCLUDE_DIR}/slice.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/status.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/db.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/export.h"

)

target_include_directories(eveldb
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        "${PROJECT_BINARY_DIR}/include"
        "${PROJECT_SOURCE_DIR}/include/eveldb"
        "${PROJECT_SOURCE_DIR}"
)

