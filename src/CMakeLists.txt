cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(cvdbg)
set(CMAKE_AUTOMOC TRUE)

set(cvdbg_sources

	cvdbg.cpp
	sequence.cpp
	repository.cpp
	scenegraph.cpp
	timeline.cpp)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt4 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})

include_directories("./")
add_definitions("-std=c++11")

add_executable(cvdbg ${cvdbg_sources})
target_link_libraries(cvdbg ${QT_LIBRARIES} ${Boost_LIBRARIES})
