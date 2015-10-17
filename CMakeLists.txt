project(hse-sample-project)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

include_directories(.)

add_library(redis
  rsync.cpp
)

add_executable(redis-server
  main.cpp
)
target_link_libraries(redis-server redis pthread)

include_directories(contrib)

add_executable(run_test
  contrib/gmock-gtest-all.cc
  contrib/gmock_main.cc
  test/rsync_test.cpp
)
target_link_libraries(run_test redis pthread)
