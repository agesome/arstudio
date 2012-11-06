cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(cvdbg)
set(CMAKE_AUTOMOC TRUE)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt4 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})

include_directories("./")
add_definitions("-std=c++11")

add_library(sequence OBJECT sequence.cpp)
add_library(repository OBJECT repository.cpp)
add_library(scenegraph OBJECT scenegraph.cpp)
add_library(timeline OBJECT timeline.cpp)


add_executable(cvdbg cvdbg.cpp sequence repository scenegraph timeline)
target_link_libraries(cvdbg ${QT_LIBRARIES} ${Boost_LIBRARIES})
