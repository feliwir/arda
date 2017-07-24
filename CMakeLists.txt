cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(PROJECT ARDA)
project(${PROJECT} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(deps)
add_subdirectory(src)
