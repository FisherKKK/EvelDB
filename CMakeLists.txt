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

# 2.1 platform environment
if (WIN32)
    set(EVELDB_PLATFORM_NAME EVELDB_PLATFORM_WINDOWS)
else (WIN32)
    set(EVELDB_PLATFORM_NAME EVELDB_PLATFORM_POSIX)
endif (WIN32)

# 3. project options
option(EVELDB_BUILD_EXAMPLES "Build example" OFF)
option(EVELDB_BUILD_TESTS "Build test cases" ON)
option(EVELDB_ENABLE_ASAN "Build with asan" OFF)

# 4. set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 4.1 inlcude extern project
include(extern/catch2/catch2.cmake)

# 4.2 cmake flag
# disable the exception
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
# now pass
else(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    string(REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

if(EVELDB_ENABLE_ASAN)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0))
        message (STATUS "Compiling with AddressSanitizer and UndefinedBehaviorSanitizer")
        set (ASAN_FLAGS "-g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize-recover=address -fno-sanitize=vptr")
    else ()
        message (STATUS "Compiling with AddressSanitizer")
        set (ASAN_FLAGS "-g -fsanitize=address -fno-omit-frame-pointer -static-libasan")
    endif ()

    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_FLAGS}")
    set (CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} ${ASAN_FLAGS}")
endif(EVELDB_ENABLE_ASAN)

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
        "util/hash.h"
        "util/testutil.h"
        "util/env.cc"
        "util/hash.cc"
        "util/coding.cc"
        "util/coding.h"
        "util/hash.h"
        "util/testutil.cc"
        "util/testutil.h"
        "port/port.h"
        "port/port_stdcxx.h"
        "port/thread_annotations.h"

    PUBLIC
        "${EVELDB_PUBLIC_INCLUDE_DIR}/slice.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/status.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/db.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/export.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/options.h"
        "${EVELDB_PUBLIC_INCLUDE_DIR}/env.h"

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

# 5.1 Platform selection
if (WIN32)
    target_sources(eveldb
        PRIVATE
            "util/env_windows.cc"    
    )
else (WIN32)
    target_sources(eveldb
        PRIVATE
            "util/env_posix.cc")
endif (WIN32)

# 5.2 add definitions
target_compile_definitions(eveldb
    PRIVATE
    ${EVELDB_PLATFORM_NAME}=1
)

# 5.2 Build test
if(EVELDB_BUILD_TESTS)
add_library(eveldb_tests "")
target_sources(eveldb_tests
    PRIVATE
    "db/skiplist_test.cc"
    "util/status_test.cc"
    "util/arena_test.cc"
)
target_link_libraries(eveldb_tests Catch2::Catch2 eveldb)
target_compile_definitions(eveldb_tests
    PRIVATE
    ${EVELDB_PLATFORM_NAME}=1)
endif(EVELDB_BUILD_TESTS)

# 5.2 add subdirectory
add_subdirectory(tests)
