cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(arstudio CXX C)

set(arstudio_sources

	main.cpp
	sequence.cpp
	repository.cpp
	scenegraph.cpp
	timelinemodel.cpp
	window3d.cpp
	point3d.cpp
	repository_view.cpp
	logger.cpp
	timeline.cpp
	core.cpp
	algo_pipeline.cpp
	config_view.cpp
	config.cpp
	video.cpp)
		
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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ui/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/algorithms/)

file(GLOB arstudio_headers ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/*.hpp)
qt4_wrap_cpp(arstudio_moc ${arstudio_headers})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-std=c++11")

add_subdirectory(algorithms)

add_executable(arstudio ${arstudio_sources} ${arstudio_moc})

target_link_libraries(
	arstudio
	adjuster

	${QT_LIBRARIES}
	${Boost_LIBRARIES}
	${OPENGL_LIBRARIES}
	${OpenCV_LIBS})
