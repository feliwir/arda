cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(PROJECT ARDA)
project(${PROJECT} VERSION 0.1 LANGUAGES CXX)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(deps)
add_subdirectory(src)
