cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

project(adjuster CXX)

set(sources adjuster.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(adjuster STATIC ${sources})
