cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(cvdbg)
set(CMAKE_AUTOMOC TRUE)

set(cvdbg_sources

	cvdbg.cpp
	sequence.cpp
	repository.cpp
	scenegraph.cpp
        timeline.cpp
        window3d.cpp
        camera.cpp
        point3d.cpp)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL)
include(${QT_USE_FILE})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

include_directories("./")
add_definitions("-std=c++11")

add_executable(cvdbg ${cvdbg_sources})
target_link_libraries(cvdbg ${QT_LIBRARIES} ${Boost_LIBRARIES} ${OPENGL_LIBRARIES})
