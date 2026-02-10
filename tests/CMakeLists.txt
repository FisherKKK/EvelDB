if(EVELDB_BUILD_TESTS)
add_executable(unittests
            test_main.cc)

target_link_libraries(unittests
                      PRIVATE
                      Catch2::Catch2
                      "-Wl,--whole-archive"
                      eveldb_tests
                      "-Wl,--no-whole-archive"
                      eveldb)

add_dependencies(unittests
                 Catch2)
endif(EVELDB_BUILD_TESTS)