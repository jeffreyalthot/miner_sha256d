cmake_minimum_required(VERSION 3.20)
project(miner_sha256d VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MINER_BUILD_TESTS "Build miner unit/integration tests" ON)

add_library(miner_core
  src/core/config.cpp
  src/core/logging.cpp
  src/core/metrics.cpp
  src/core/runtime.cpp
  src/sv2/framing.cpp
  src/sv2/messages.cpp
  src/sv2/session.cpp
  src/work/job.cpp
  src/work/merkle.cpp
  src/work/target.cpp
  src/hash/sha256.cpp
  src/hash/sha256d.cpp
  src/mining/scheduler.cpp
  src/mining/worker.cpp
  src/mining/submit.cpp
)

target_include_directories(miner_core PUBLIC include)
target_compile_options(miner_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(miner src/main.cpp)
target_link_libraries(miner PRIVATE miner_core)

if(MINER_BUILD_TESTS)
  enable_testing()
  add_executable(miner_tests src/tests/test_main.cpp)
  target_link_libraries(miner_tests PRIVATE miner_core)
  add_test(NAME miner_tests COMMAND miner_tests)
endif()
