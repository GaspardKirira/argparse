cmake_minimum_required(VERSION 3.20)
project(argparse LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(argparse INTERFACE)
add_library(argparse::argparse ALIAS argparse)

target_include_directories(argparse INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if (MSVC)
  target_compile_options(argparse INTERFACE /W4 /permissive-)
else()
  target_compile_options(argparse INTERFACE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
enable_testing()

add_executable(argparse_basic_test tests/test_basic.cpp)
target_link_libraries(argparse_basic_test PRIVATE argparse::argparse)
add_test(NAME argparse.basic COMMAND argparse_basic_test)
