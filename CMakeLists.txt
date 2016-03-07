cmake_minimum_required (VERSION 2.6)
project (parsical)

set (CMAKE_EXPORT_COMPILE_COMMANDS on)

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_DIR}/third-party/)

add_subdirectory(src)

