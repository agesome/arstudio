cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(arstudio CXX C)

set(arstudio_sources

	ui/main.cpp
	ui/timelinemodel.cpp
	ui/repository_view.cpp
	ui/timeline.cpp
	ui/processing_dialog.cpp
	ui/window3d.cpp
	ui/core.cpp
	ui/config_editor.cpp
	
	algo_pipeline.cpp
	sequence.cpp
	repository.cpp
	scenegraph.cpp
	point3d.cpp
	logger.cpp
	config.cpp
)
		
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL QtXml)
include(${QT_USE_FILE})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(OpenCV REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/algorithms/)

file(GLOB arstudio_headers ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/*.hpp)
qt4_wrap_cpp(arstudio_moc ${arstudio_headers})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-std=c++11 -O0 -ggdb3")

add_subdirectory(algorithms)

add_executable(arstudio ${arstudio_sources} ${arstudio_moc})

target_link_libraries(
	arstudio
	adjuster

	${QT_LIBRARIES}
	${Boost_LIBRARIES}
	${OPENGL_LIBRARIES}
	${OpenCV_LIBS})
