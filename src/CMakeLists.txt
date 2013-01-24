cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(cvdbg CXX C)
set(CMAKE_AUTOMOC TRUE)

set(cvdbg_sources

	cvdbg.cpp
	sequence.cpp
	repository.cpp
	scenegraph.cpp
	timelinemodel.cpp
	window3d.cpp
	camera.cpp
	point3d.cpp
	repository_view.cpp
	logger.cpp
	timeline.cpp
	core.cpp
	algo_pipeline.cpp
	config.cpp
	config_view.cpp)
	
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL QtXml)
include(${QT_USE_FILE})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(OpenCV REQUIRED)

find_library(lib_ltdl ltdl)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(cvdbg ${cvdbg_sources})
target_link_libraries(cvdbg
	${QT_LIBRARIES}
	${Boost_LIBRARIES}
	${OPENGL_LIBRARIES}
	${OpenCV_LIBS}
	${lib_ltdl})

#add_subdirectory (ClassKin)
add_subdirectory (modules)
