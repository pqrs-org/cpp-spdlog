cmake_minimum_required (VERSION 3.9)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall)
add_compile_options(-Werror)
add_compile_options(-O2)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../../include)
include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR}/../vendor/include)

project (test)

add_executable(
  test
  factory_test.cpp
  monitor_test.cpp
  read_log_files_test.cpp
  test.cpp
  unique_filter_test.cpp
)
