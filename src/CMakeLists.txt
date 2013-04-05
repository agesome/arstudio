cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(arstudio CXX C)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
  	"Choose the type of build: Debug/Release" FORCE)
endif()

set(arstudio_sources

	ui/main.cpp
	ui/timelinemodel.cpp
	ui/repository_view.cpp
	ui/timeline.cpp
	ui/processing_dialog.cpp
	ui/window3d.cpp
	ui/core.cpp
	ui/config_editor.cpp
	ui/window2d.cpp

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
find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL QtXml)
include(${QT_USE_FILE})
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../submodules/KinectRecorder/src/
	${OPENGL_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include/
	${GLOBAL_INCLUDES}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../submodules/KinectRecorder/src/
		${CMAKE_CURRENT_BINARY_DIR}/KinectRecorder EXCLUDE_FROM_ALL)

file(GLOB arstudio_headers ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/*.hpp)
qt4_wrap_cpp(arstudio_moc ${arstudio_headers})

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -ggdb3 -Wall -Wextra -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
endif()

set(GLOBAL_LIBS "")
set(ALGO_INCLUDES "")
set(ALGO_CODE "")

macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        set(dirlist ${dirlist} ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

subdirlist(subdirs ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/)

message (STATUS "Will now add algorithms")
foreach(subdir ${subdirs})
	message (STATUS "  Adding algorithm "${subdir})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/algorithms/${subdir}/)
	if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/${subdir}/include/)
		include_directories(${CMAKE_CURRENT_SOURCE_DIR}/algorithms/${subdir}/include/)
	endif ()
	if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/${subdir}/src/)
		include_directories(${CMAKE_CURRENT_SOURCE_DIR}/algorithms/${subdir}/src/)
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/algorithms/${subdir}/src/
				${CMAKE_CURRENT_BINARY_DIR}/${subdir} EXCLUDE_FROM_ALL)
	else ()
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/algorithms/${subdir}/
				${CMAKE_CURRENT_BINARY_DIR}/${subdir} EXCLUDE_FROM_ALL)
	endif ()
endforeach()

set(CONFIG_DIRECTORY "../algorithms/")

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/ui/processing_dialog.cpp.in
				${CMAKE_CURRENT_SOURCE_DIR}/ui/processing_dialog.cpp)

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/include/algo_pipeline.hpp.in
				${CMAKE_CURRENT_SOURCE_DIR}/include/algo_pipeline.hpp)

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/algo_pipeline.cpp.in
				${CMAKE_CURRENT_SOURCE_DIR}/algo_pipeline.cpp)

message(STATUS "Build type is: ${CMAKE_BUILD_TYPE}")

add_executable(arstudio ${arstudio_sources} ${arstudio_moc})
target_link_libraries(
	arstudio

	kinectCapture
	${GLOBAL_LIBS}
	${QT_LIBRARIES}
	${Boost_LIBRARIES}
	${OPENGL_LIBRARIES}
	${OpenCV_LIBS}
)
