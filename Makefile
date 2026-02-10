CMAKE_GENERATOR ?= "Unix Makefiles"
COMPILE_JOBS ?= 6
DEBUG_BUILD_DIR ?= "./build/"
RELEASE_BUILD_DIR ?= "./build-release/"
EVELDB_VERBOSE ?= "--verbose"

EVELDB_CMAKE_ARGS := -DCMAKE_EXPORT_COMPILE_COMMANDS=1
EVELDB_CMAKE_ARGS := ${EVELDB_CMAKE_ARGS}

EVELDB_CMAKE_ARGS := ${EVELDB_CMAKE_ARGS} -G ${CMAKE_GENERATOR} -S.


.PHONY: config
config:                 ## Just configure the project
	cmake ${EVELDB_CMAKE_ARGS} -B${DEBUG_BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug -DEVELDB_ENABLE_ASAN=OFF

.PHONY: debug
debug:                   ## Build with debug options.
	cmake ${EVELDB_CMAKE_ARGS} -B${DEBUG_BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug -DEVELDB_ENABLE_ASAN=OFF
	cmake --build ${DEBUG_BUILD_DIR} --parallel ${COMPILE_JOBS} ${EVELDB_VERBOSE}

.PHONY: asan
asan:                   ## Build with debug options.
	cmake ${EVELDB_CMAKE_ARGS} -B${DEBUG_BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug -DEVELDB_ENABLE_ASAN=ON
	cmake --build ${DEBUG_BUILD_DIR} --parallel ${COMPILE_JOBS} ${EVELDB_VERBOSE}


.PHONY: fmt
fmt:
	find . -iname '*.cc' -o -iname '*.h' -o -iname '*.h.in' | xargs clang-format -i --style=file