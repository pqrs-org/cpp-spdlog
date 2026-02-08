cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall)
add_compile_options(-Werror)
add_compile_options(-O2)
add_compile_options(-DSPDLOG_COMPILED_LIB)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../../include)
include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR}/../../vendor/vendor/include)

add_library(libspdlog.a STATIC IMPORTED)
set_target_properties(libspdlog.a PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/../../src/spdlog-build/build/Release/libspdlog.a)

project (test)

add_executable(
  test
  test.cpp
)

target_link_libraries(
  test
  libspdlog.a
)
