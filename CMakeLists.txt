cmake_minimum_required (VERSION 2.6)
project (parsical)

set (CMAKE_EXPORT_COMPILE_COMMANDS on)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wextra")

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_SOURCE_DIR}/third-party/)

add_subdirectory(src)

