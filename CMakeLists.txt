cmake_minimum_required(VERSION 2.6)
project(LD28)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external ${CMAKE_CURRENT_BINARY_DIR}/external)

add_subdirectory(external/Kunlaboro)
add_subdirectory(src)