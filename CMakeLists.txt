cmake_minimum_required(VERSION 3.17)
project(SimpleClientServer)

set(CMAKE_CXX_STANDARD 11)
set(CURRENT_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SIMPLE_LIB_NAME client-server)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example)