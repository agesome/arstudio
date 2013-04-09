cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(arstudio CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_SKIP_RULE_DEPENDENCY ON)
set(Boost_USE_MULTITHREADED ON)
set(GLOBAL_LIBS "")
set(ALGO_INCLUDES "")
set(ALGO_CODE "")

set(arstudio_sources
	Config/private/Config.cpp
	ConfigEditor/private/ConfigEditor.cpp
	Core/private/Core.cpp
	Logger/private/Logger.cpp
	main.cpp
	Repository/private/Repository.cpp
	RepositoryView/private/RepositoryView.cpp
	Scenegraph/private/Scenegraph.cpp
	Sequence/private/Point3d.cpp
	Sequence/private/Sequence.cpp
	TimeLine/private/TimeLine.cpp
	TimeLine/private/TimeLineModel.cpp
	Window2D/private/Window2D.cpp
	Window3D/private/Window3D.cpp

#	generated with process_file
	${CMAKE_BINARY_DIR}/ProcessingDialog.cpp
	${CMAKE_BINARY_DIR}/AlgoPipeline.cpp
)

set (arstudio_headers
	ConfigEditor/ConfigEditor.hpp
	Core/Core.hpp
	RepositoryView/RepositoryView.hpp
	TimeLine/TimeLine.hpp
	TimeLine/TimeLineModel.hpp
	Window2D/Window2D.hpp
	Window3D/Window3D.hpp
	ProcessingDialog/ProcessingDialog.hpp
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
	"Choose the type of build: Debug/Release" FORCE)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -ggdb3 -Wall -Wextra -pedantic")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
endif()

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL QtXml)
include(${QT_USE_FILE})
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../submodules/KinectRecorder/src/
		${CMAKE_CURRENT_BINARY_DIR}/KinectRecorder EXCLUDE_FROM_ALL)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/ProcessingDialog
	${CMAKE_CURRENT_SOURCE_DIR}/AlgoPipeline
	${CMAKE_CURRENT_SOURCE_DIR}/Logger
	${CMAKE_CURRENT_SOURCE_DIR}/Config
	${CMAKE_CURRENT_SOURCE_DIR}/ConfigEditor
	${CMAKE_CURRENT_SOURCE_DIR}/Core
	${CMAKE_CURRENT_SOURCE_DIR}/Repository
	${CMAKE_CURRENT_SOURCE_DIR}/RepositoryView
	${CMAKE_CURRENT_SOURCE_DIR}/Scenegraph
	${CMAKE_CURRENT_SOURCE_DIR}/Sequence
	${CMAKE_CURRENT_SOURCE_DIR}/TimeLine
	${CMAKE_CURRENT_SOURCE_DIR}/Window2D
	${CMAKE_CURRENT_SOURCE_DIR}/Window3D
	${CMAKE_CURRENT_SOURCE_DIR}/../submodules/KinectRecorder/src/
	${OPENGL_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
	${GLOBAL_INCLUDES}
)

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
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/ProcessingDialog/private/ProcessingDialog.cpp.in
				${CMAKE_BINARY_DIR}/ProcessingDialog.cpp)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/AlgoPipeline/AlgoPipeline.hpp.in
				${CMAKE_BINARY_DIR}/AlgoPipeline.hpp)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/AlgoPipeline/private/AlgoPipeline.cpp.in
				${CMAKE_BINARY_DIR}/AlgoPipeline.cpp)

qt4_wrap_cpp(arstudio_moc ${arstudio_headers})

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
