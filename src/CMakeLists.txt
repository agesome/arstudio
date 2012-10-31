cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(cvdbg)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

include_directories ("./")
add_definitions ("-std=c++11")

add_library(sequence OBJECT sequence.cpp)
add_library(repository OBJECT repository.cpp)

add_executable(cvdbg cvdbg.cpp sequence repository ${Boost_LIBRARIES})