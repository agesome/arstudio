cmake_minimum_required(VERSION 2.8.6 FATAL_ERROR)

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
  main.cpp
)

# this is somewhat tricky. we want a separate list of all headers so that they
# show up for those who use Qt Creator, and we want a list of Qt-using class
# headers to process them with moc; we cannot just process all the headers,
# that will produce warnings

set (arstudio_headers
  Config/Config.hpp
  Logger/Logger.hpp
  Repository/Repository.hpp
  Scenegraph/Scenegraph.hpp
  Sequence/Point3D.hpp
  Sequence/Sequence.hpp
  Sequence/Bitmap.hpp
  Sequence/Camera.hpp
  Sequence/Point3D.hpp
  Sequence/PointCloud.hpp
  ConfigEditor/ConfigEditor.hpp
  Core/Core.hpp
  RepositoryView/RepositoryView.hpp
  Sequence/Point3D.hpp
  TimeLine/TimeLine.hpp
  TimeLine/TimeLineModel.hpp
  Window2D/Window2D.hpp
  Window3D/Window3D.hpp
  ProcessingDialog/ProcessingDialog.hpp
)

set (arstudio_moc_headers
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

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5OpenGL_DEFINITIONS})
add_definitions(${Qt5Concurrent_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5OpenGL_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Concurrent_EXECUTABLE_COMPILE_FLAGS}")

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -ggdb3 -Wall -Wextra -pedantic")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
endif()

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
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5OpenGL_INCLUDE_DIRS}
  ${Qt5Concurrent_INCLUDE_DIRS}
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

if(NOT ALGORITHMS)
  subdirlist(subdirs ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/)
    set(ALGORITHMS ${subdirs} CACHE STRING
    "List algorithms to build" FORCE)
endif()

message (STATUS "Algorithms to build: " ${ALGORITHMS})

if (NOT ALGORITHMS STREQUAL "")
  message (STATUS "Will now add algorithms")
  foreach(subdir ${ALGORITHMS})
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
endif ()

set(CONFIG_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/")
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/ProcessingDialog/private/ProcessingDialog.cpp
        ${CMAKE_BINARY_DIR}/ProcessingDialog.cpp)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/AlgoPipeline/AlgoPipeline.hpp
        ${CMAKE_BINARY_DIR}/AlgoPipeline.hpp)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/AlgoPipeline/private/AlgoPipeline.cpp
        ${CMAKE_BINARY_DIR}/AlgoPipeline.cpp)

qt5_wrap_cpp(arstudio_moc ${arstudio_moc_headers})

message(STATUS "Build type is: ${CMAKE_BUILD_TYPE}")

add_executable(arstudio ${arstudio_sources} ${arstudio_headers} ${arstudio_moc})
target_link_libraries(
  arstudio

  ${GLOBAL_LIBS}
  ${Qt5Widgets_LIBRARIES}
  ${Qt5OpenGL_LIBRARIES}
  ${Qt5Concurrent_LIBRARIES}
  ${Boost_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${OpenCV_LIBS}
)
